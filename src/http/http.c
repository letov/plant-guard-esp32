#include <esp_http_server.h>
#include <esp_log.h>
#include "config.h"
#include "http/routing/base.h"
#include "http/routing/static.h"

httpd_handle_t http_init(void) {
    ESP_LOGI(TAG, "start http server");

    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    httpd_handle_t server = NULL;

    if (httpd_start(&server, &config) == ESP_OK) {
        base_routing_register(server);
        static_routing_register(server);
    }

    return server;
}