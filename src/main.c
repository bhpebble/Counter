#include <pebble.h>

Window *my_window;
TextLayer *text_layer;
int g_count = 0;
char buffer [10];

/*
int num_digits(int num)
{
  if (num == 0)
    return 1;
  else
  {
    int i = 0;
    while (num > 0)
    {
      num = num / 10;
      i++;
    }
    return i;
  }
}
*/

void up_click_handler(ClickRecognizerRef recognizer, void *context)
{ 
  TextLayer *tl = (TextLayer *)context;
  g_count++;
  
  vibes_short_pulse();
  
  snprintf(buffer, 10, "%d", g_count);
  text_layer_set_text(tl, buffer);
}
 
void down_click_handler(ClickRecognizerRef recognizer, void *context)
{
  g_count--;

  vibes_short_pulse();
  
  snprintf(buffer, 10, "%d", g_count);
  text_layer_set_text(text_layer, buffer);
}

void long_select_handler(ClickRecognizerRef recognizer, void *context)
{
  g_count = 0;
  
  snprintf(buffer, 10, "%d", g_count);
  text_layer_set_text(text_layer, buffer);
}
 
void click_config_provider(void *context)
{
    window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
    window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
    window_long_click_subscribe(BUTTON_ID_SELECT, 1000, long_select_handler, NULL);
}

void window_load(Window *window)
{
  ResHandle font_handle = resource_get_handle(RESOURCE_ID_IMAGINE_42);

  Layer *window_layer = window_get_root_layer(window);
  GRect window_bounds = layer_get_bounds(window_layer);
  
  text_layer = text_layer_create(GRect(0, 53, window_bounds.size.w, window_bounds.size.h));
  text_layer_set_background_color(text_layer, GColorClear);
  text_layer_set_text_color(text_layer, GColorBlack);
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
  text_layer_set_font(text_layer, fonts_load_custom_font(font_handle));
  window_set_click_config_provider_with_context(my_window, click_config_provider, text_layer);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(text_layer));
 
  text_layer_set_text(text_layer, "0");

}
 
void window_unload(Window *window)
{
  text_layer_destroy(text_layer);
}

void handle_init(void) {
  my_window = window_create();

  window_set_window_handlers(my_window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  
  window_stack_push(my_window, true);
}

void handle_deinit(void) {
  window_destroy(my_window);
}

int main(void) {
  handle_init();
  app_event_loop();
  handle_deinit();
}
