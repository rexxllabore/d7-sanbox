<?php

/**
 * @file
 * Theme functions for Slick Views.
 * Hooks and preprocess functions for the slick themes module.
 */

/**
 * Implements hook_preprocess_HOOK().
 */
function template_preprocess_slick_views(&$variables) {
  $element = array();

  // Prepare all data to be passed to theme_slick() via theme_slick_views().
  if (!empty($variables['view'])) {
    $view         = $variables['view'];
    $handler      = $view->style_plugin;
    $settings     = $variables['options'];
    $row_plugin   = $view->style_plugin->row_plugin;
    $attach       = array('attach_media' => FALSE, 'attach_skin' => NULL);
    $items        = array();

    foreach ($variables['rows'] as $id => $row) {
      // Sets row_index to get the rendered fields.
      $view->row_index = $id;
      $item['item']['type'] = 'views';
      $item['item']['view_name'] = $view->name;

      if (!empty($settings['slide_field_wrapper'])) {
        $slide_image = $settings['slide_image'];

        // Main image, if so configured.
        if (isset($row->image) && !empty($row->image)) {
          $attach['attach_media'] = TRUE;
          $item['slide'] = $row->image;

          if (isset($row->thumbnail) && !empty($row->thumbnail)) {
            $settings['thumbnail_style'] = TRUE;
            $item['thumbnail'] = $row->thumbnail;
          }
        }

        // Caption fields.
        $item['caption'] = array();
        if (isset($row->caption) && !empty($row->caption)) {
          $item['caption']['data'] = $row->caption;
        }

        // Overlay, if so configured.
        if (isset($row->overlay) && !empty($row->overlay)) {
          $attach['attach_media'] = TRUE;
          $item['caption']['overlay'] = $row->overlay;
        }

        // Title, if so configured.
        if (isset($row->title) && !empty($row->title)) {
          $item['caption']['title'] = check_plain($row->title);
        }

        // Link, if so configured.
        if (isset($row->link) && !empty($row->link)) {
          $item['caption']['link'] = $row->link;
        }

        // Layout, if so configured.
        if (isset($row->layout) && !empty($row->layout)) {
          $item['caption']['layout'] = check_plain($row->layout);
        }
      }
      else {
        // @todo refine more.
        // $attach['attach_colorbox'] = TRUE;
        $attach['attach_media'] = TRUE;
        $item['slide'] = $row_plugin->render($row);
      }

      $items[] = $item;
    }

    // Add the settings.
    if ($settings['skin']) {
      $attach['attach_skin'] = $settings['skin'];
    }

    if ($settings['id']) {
      $settings['attributes']['id'] = $settings['id'];
    }
    $settings['attributes']['class'][] = drupal_clean_css_identifier('slick--view--' . $view->name);

    // Just dance with Views provided variables.
    $variables['rows'] = $items;
    $variables['options'] = $settings;
    $variables['attach'] = slick_attach($attach);
  }
}
