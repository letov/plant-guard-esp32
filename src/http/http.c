#include <esp_http_server.h>
#include <esp_log.h>
#include "config.h"
#include "http/route/base.h"
#include "http/route/static.h"

#define TAG "HTTP"

httpd_handle_t http_init(void) {
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    httpd_handle_t server = NULL;

    if (httpd_start(&server, &config) == ESP_OK) {
        base_routing_register(server);
        static_routing_register(server);
    }

    ESP_LOGI(TAG, "start http server...OK");
    return server;
}