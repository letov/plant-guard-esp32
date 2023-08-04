#include <esp_http_server.h>

esp_err_t health_handler(httpd_req_t *req) {
    return httpd_resp_send(req, "OK", HTTPD_RESP_USE_STRLEN);
}

httpd_uri_t uri_health = {
        .uri = "/health",
        .method = HTTP_GET,
        .handler = health_handler,
        .user_ctx = NULL
};

void base_routing_register(httpd_handle_t server) {
    httpd_register_uri_handler(server, &uri_health);
}