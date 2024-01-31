#include <stdlib.h>

#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/led.h>
#include <zephyr/kernel.h>
#include <zephyr/init.h>

#include <zmk/ble.h>
#include <zmk/endpoints.h>
#include <zmk/keymap.h>
#include <zmk/split/bluetooth/peripheral.h>
#include <zmk/battery.h>
#include <zmk/event_manager.h>
#include <zmk/events/keycode_state_changed.h>
#include <zmk/events/layer_state_changed.h>

#include <zephyr/logging/log.h>

LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

const struct device *dev = zmk_behavior_get_binding(binding->behavior_dev);
struct behavior_auto_correct_data *data = dev->correction_buffer ;

// inits main struct
static int auto_correct_init(const struct device *dev) {
    // TODO: create log here to know the mod has started
    return 0;
}

// manages new keycodes presses
ZMK_LISTENER(behavior_auto_correct, auto_correct_keycode_changed);
ZMK_SUBSCRIPTION(behavior_auto_correct, zmk_position_state_changed);
int keycode_state_changed(const zmk_event_t *eh) {
    
}
