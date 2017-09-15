<?php
$t = array( 'start'=>microtime(true), 'bootstrap'=>null, 'fin'=>null );


header('X-Debug-Stat: P', false);

$t = array( 'start'=>microtime(true), 'bootstrap'=>null, 'fin'=>null );

// Register our shutdown function so that no other shutdown functions run before this one.
// This shutdown function calls exit(), immediately short-circuiting any other shutdown functions,
// such as those registered by the devel.module for statistics.
register_shutdown_function('status_shutdown');
function status_shutdown() {
  global $t;
  $t['fin'] = microtime(true);
  if ( !empty($_GET['timers']) )
  {
    echo " start({$t['start']}) bootstrap({$t['bootstrap']}) fin({$t['fin']})";
  }
  exit();
}

if ( !empty($_GET['phponly']) )
{
    exit();
}


// Build up our list of errors.
$errors = array();
define('DRUPAL_ROOT', getcwd());
// Drupal bootstrap.
require_once './includes/bootstrap.inc';


try {
    drupal_bootstrap(DRUPAL_BOOTSTRAP_VARIABLES);
}
catch (Exception $e) {
    $errors[]= $e->getMessage();
}
$t['bootstrap'] = microtime(true);

header('X-Debug-Stat: B', false);

//  Check that the Drupal site is in  Maintenance Mode
if (variable_get('maintenance_mode', 0)) {
    $errors[] = 'Drupal site has been put into Maintenance Mode.';
}

// Print all errors.
if ($errors) {
  $errors[] = 'Errors on this server will cause it to be removed from the load balancer.';
  header('HTTP/1.1 503 Internal Server Error');
  print implode("<br />\n", $errors);
}
else {
  print 'Status Ok';
}

// Exit immediately, note the shutdown function registered at the top of the file.
exit();

