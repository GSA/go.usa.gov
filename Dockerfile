FROM ctac/drupal-base:7.1

# Inject Consul Template
ENV CONSUL_TEMPLATE_VERSION 0.15.0

COPY app/drupal/sites /var/www/html/sites
COPY app/drupal/misc /var/www/html/misc
COPY docker/drupal/rootfs/etc/nginx/sites-include /etc/nginx/sites-include
RUN chown -R www-data:www-data /var/www/html

## S6 managed consul-template to generate env files

ADD https://releases.hashicorp.com/consul-template/${CONSUL_TEMPLATE_VERSION}/consul-template_${CONSUL_TEMPLATE_VERSION}_linux_amd64.zip /

RUN unzip -d / /consul-template_${CONSUL_TEMPLATE_VERSION}_linux_amd64.zip && \
    mv /consul-template /usr/local/bin/consul-template &&\
    rm -rf /consul-template_${CONSUL_TEMPLATE_VERSION}_linux_amd64.zip && \
    mkdir -p /consul-template /consul-template/config.d /consul-template/templates

COPY consul-template/config.d /consul-template/config.d/
COPY consul-template/templates /consul-template/templates/

COPY s6 /etc/services.d/

RUN apt-get clean && rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/*
