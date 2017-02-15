FROM ctac/drupal-base:7.54-php7.1

COPY app/drupal/sites /var/www/html/sites
COPY app/drupal/misc  /var/www/html/misc

# for dev only
COPY docker/drupal/rootfs /
COPY docker/drupal/rootfs/etc/php/dev /etc/php/${PHP_VERSION}
