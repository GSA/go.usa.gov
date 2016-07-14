<?php

$databases = array (
  'default' => array (
    'default' => array (
      'database'  => getenv('RDS_DB_NAME'),
      'username'  => getenv('RDS_USERNAME'),
      'password'  => getenv('RDS_PASSWORD'),
      'host'      => getenv('RDS_HOSTNAME'),
      'port'      => getenv('RDS_PORT'),
      'driver'    => getenv('RDS_DRIVER'),
      'prefix'    => getenv('RDS_DB_PREFIX'),
      'collation' => getenv('RDS_COLLATION'),
    ),
  ),
  'metrics' => array (
    'default' => array (
      'database'  => getenv('RDS_DB_NAME_METRICS'),
      'username'  => getenv('RDS_USERNAME'),
      'password'  => getenv('RDS_PASSWORD'),
      'host'      => getenv('RDS_HOSTNAME'),
      'port'      => getenv('RDS_PORT'),
      'driver'    => getenv('RDS_DRIVER'),
      'prefix'    => getenv('RDS_DB_PREFIX'),
      'collation' => getenv('RDS_COLLATION'),
    ),
  ),
);
$databases['default']['metrics'] =& $databases['metrics']['default'];
$drupal_hash_salt = getenv('DRUPAL_HASHSALT');
$cookie_domain    = '.usa.dev';

$update_free_access = FALSE;
ini_set('session.gc_probability',  1);
ini_set('session.gc_divisor',      100);
ini_set('session.gc_maxlifetime',  200000);
ini_set('session.cookie_lifetime', 2000000);
$conf['404_fast_paths_exclude'] = '/\/(?:styles)\//';
$conf['404_fast_paths'] = '/\.(?:txt|png|gif|jpe?g|css|js|ico|swf|flv|cgi|bat|pl|dll|exe|asp)$/i';
$conf['404_fast_html']  = '<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML+RDFa 1.0//EN" "http://www.w3.org/MarkUp/DTD/xhtml-rdfa-1.dtd"><html xmlns="http://www.w3.org/1999/xhtml"><head><title>404 Not Found</title></head><body><h1>Not Found</h1><p>The requested URL "@path" was not found on this server.</p></body></html>';

$conf['drupal_http_request_fails'] = FALSE;

foreach ( $_ENV as $__var=>$__val )
{
    if ( strlen($__var)>7 && strtolower(substr($__var,0,7)) === 'drupal_' )
    {
        $conf[strtolower(substr($__var,7))] = $__val;
    }
}
unset($__var);
unset($__val);
