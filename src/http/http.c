#include <esp_http_server.h>
#include <esp_log.h>
#include "config.h"

esp_err_t get_req_handler(httpd_req_t *req) {
    return httpd_resp_send(req, "OK", HTTPD_RESP_USE_STRLEN);
}

httpd_uri_t uri_get = {
        .uri = "/health",
        .method = HTTP_GET,
        .handler = get_req_handler,
        .user_ctx = NULL
};

httpd_handle_t http_init(void) {
    ESP_LOGI(TAG, "start http server");

    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    httpd_handle_t server = NULL;

    if (httpd_start(&server, &config) == ESP_OK) {
        httpd_register_uri_handler(server, &uri_get);
    }

    return server;
}