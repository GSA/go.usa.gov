FROM ubuntu:trusty

ARG PHP_VERSION
ENV PHP_VERSION ${PHP_VERSION:-7.3}

ARG DRUPAL_VERSION
ENV DRUPAL_VERSION ${DRUPAL_VERSION:-7.63}

RUN apt-get update \
    && DEBIAN_FRONTEND=noninteractive apt-get install -y --force-yes --no-install-recommends \
      locales \
    && locale-gen en_US.UTF-8 \
    && echo "LANG   en_US.UTF-8" >> /etc/default/locale \
    && echo "LC_ALL en_US.UTF-8" >> /etc/default/locale \
    && DEBIAN_FRONTEND=noninteractive apt-get install -y --force-yes --no-install-recommends \
        software-properties-common unattended-upgrades curl git \
    && unattended-upgrade -d \
    && add-apt-repository ppa:ondrej/php \
    && add-apt-repository ppa:nginx/stable \
    && apt-get update \
    && DEBIAN_FRONTEND=noninteractive apt-get install -y --force-yes --no-install-recommends \
        make php${PHP_VERSION}-dev gettext-base unzip \
        mysql-client \
        php${PHP_VERSION}-cli php${PHP_VERSION}-fpm \
        php${PHP_VERSION}-bcmath \
        php${PHP_VERSION}-bz2 \
        php${PHP_VERSION}-curl \
        php${PHP_VERSION}-gd \
        php${PHP_VERSION}-intl \
        php${PHP_VERSION}-json \
        php${PHP_VERSION}-ldap \
        php${PHP_VERSION}-mbstring \
        php${PHP_VERSION}-mysql \
        php${PHP_VERSION}-opcache \
        php${PHP_VERSION}-readline \
        php${PHP_VERSION}-recode \
        php${PHP_VERSION}-xml \
        php${PHP_VERSION}-xsl \
        php${PHP_VERSION}-zip \
        php-igbinary \
        php-redis \
        php-pear \
        nginx \
    && ln -s /etc/php/${PHP_VERSION} /etc/php/$(echo $PHP_VERSION | grep -o -E '[0-9]+' | head -1 | sed -e 's/^0\+//') \
    && ln -s /usr/sbin/php-fpm${PHP_VERSION} /usr/sbin/php-fpm \
    && ln -s /etc/init.d/php${PHP_VERSION}-fpm /etc/init.d/php-fpm \
    && ln -s /etc/php/${PHP_VERSION} /etc/php/curr \
    && mkdir -p -m 664 /var/lib/php/sessions /var/log/app/nginx /var/log/app/php \
        && chown -R www-data:www-data /var/lib/php/sessions /var/log/app/nginx /var/log/app/php /var/www/html \
    && apt-get -y autoclean \
        && apt-get -y clean \
        && apt-get -y autoremove \
        && rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/*

RUN cd /usr/src \
    && curl -sS https://getcomposer.org/installer | php \
    && mv composer.phar /usr/local/bin/composer \
    && chmod a+x /usr/local/bin/composer \
    && composer global require drush/drush ~8.1 \
    && composer global update --no-plugins --no-scripts \
    && ln -s /root/.composer/vendor/bin/drush /usr/local/bin/drush \
    && chmod a+x /usr/local/bin/drush /root/.composer/vendor/bin/drush \
    && cd /var/www \
    && drush dl -y drupal-$DRUPAL_VERSION  \
        --destination=/var/www --drupal-project-rename=html \
    && chown -R www-data:www-data /var/www/html \
    && mkdir -p /etc/nginx/certs \
    && openssl req \
        -new -newkey rsa:2048 \
        -days 365 -nodes -x509 \
        -subj "/C=US/ST=Virginia/L=Falls Church/O=CTAC/CN=localhost" \
        -keyout /etc/nginx/certs/site.key \
        -out    /etc/nginx/certs/site.cert

COPY docker/drupal/rootfs/ /
COPY app/drupal/sites /var/www/html/sites
COPY app/drupal/misc  /var/www/html/misc
COPY app/drupal/status.php  /var/www/html/status.php

RUN tar xfz /tmp/s6-overlay-amd64.tar.gz -sC / \
    && cp -R /etc/php/all/* /etc/php/${PHP_VERSION}/
    
EXPOSE 80 443

WORKDIR /var/www/html

CMD ["/init"]