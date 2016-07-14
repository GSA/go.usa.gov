<?php

/**
 * @file
 * Shurly Views handler for test link
 */

/**
 * Field handler to present a link to the short URL entry.
 */
class shurly_handler_field_shurly_link_test extends views_handler_field {
  function construct() {
    parent::construct();
  }

  function option_definition() {
    $options = parent::option_definition();

    $options['text'] = array('default' => '', 'translatable' => TRUE);

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['text'] = array(
      '#type' => 'textfield',
      '#title' => t('Text to display'),
      '#default_value' => $this->options['text'],
    );
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    $text = !empty($this->options['text']) ? $this->options['text'] : t('test');
    $source = $values->shurly_source;
    return _sl($text, $source, array('language'));
  }
}
