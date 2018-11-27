#include "main/app.h"

static const char *TAG = "APP";

void app_main()
{
    gpio_install_isr_service(0);

    esp_log_level_set("*", ESP_LOG_INFO);
    esp_log_level_set("OTA", ESP_LOG_VERBOSE);

    WIFI_init(WIFI_MODE_STA , NULL);
    OTA_init();

    ESP_LOGI(TAG, "OTA_init() done");

    while (1) {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }

    vTaskDelete(NULL);
}
