#include <esp_http_server.h>
#include <esp_log.h>
#include "http/model/settings/settings.h"

#define TAG "ROUTE"

esp_err_t set_api_settings_handler(httpd_req_t *req) {
    size_t content_len = req->content_len;
    char *buffer = (char *) malloc(content_len + 1);

    if (!buffer) {
        ESP_LOGI(TAG, "malloc settings buffer...FAILED");
        httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Some error");
        return ESP_OK;
    }

    httpd_req_recv(req, buffer, content_len);
    buffer[content_len] = '\0';
    ESP_LOGI(TAG, "get %d bytes: %.*s", content_len, content_len, buffer);

    if (! is_settings_valid(buffer)) {
        free(buffer);
        ESP_LOGI(TAG, "validate settings json...FAILED");
        httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "Bad Request");
        return ESP_OK;
    }

    if (save_settings_to_nvs(buffer)) {
        free(buffer);
        return httpd_resp_send(req, "OK", HTTPD_RESP_USE_STRLEN);
    }

    free(buffer);
    httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Some error");
    return ESP_OK;
}

httpd_uri_t uri_set_api_settings = {
        .uri = "/api/settings",
        .method = HTTP_PUT,
        .handler = set_api_settings_handler,
        .user_ctx = NULL
};

esp_err_t get_api_settings_handler(httpd_req_t *req) {
    char *settings = read_settings_from_nvs();

    if (! settings) {
        ESP_LOGI(TAG, "set default settings");
    }

    httpd_resp_set_type(req, "application/json");
    httpd_resp_send(
            req,
            settings ? settings : get_default_settings(),
            HTTPD_RESP_USE_STRLEN);
    free(settings);

    return ESP_OK;
}

httpd_uri_t uri_get_api_settings = {
        .uri = "/api/settings",
        .method = HTTP_GET,
        .handler = get_api_settings_handler,
        .user_ctx = NULL
};

esp_err_t health_handler(httpd_req_t *req) {
    httpd_resp_send(req, "OK", HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

httpd_uri_t uri_health = {
        .uri = "/health",
        .method = HTTP_GET,
        .handler = health_handler,
        .user_ctx = NULL
};

void base_routing_register(httpd_handle_t server) {
    httpd_register_uri_handler(server, &uri_get_api_settings);
    httpd_register_uri_handler(server, &uri_set_api_settings);
    httpd_register_uri_handler(server, &uri_health);
}