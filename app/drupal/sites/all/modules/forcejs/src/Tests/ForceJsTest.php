<?php

/**
 * @file
 * Contains \Drupal\forcejs\Tests\ForceJsTest.
 */

namespace Drupal\forcejs\Tests;

/**
 * Test cases for the ForceJs module.
 */
class ForceJsTest extends \DrupalWebTestCase {

  protected $profile = 'testing';

  /**
   * {@inheritdoc}
   */
  public static function getInfo() {
    return array(
      'name' => 'Force JS',
      'description' => 'Test cases for the ForceJs module',
      'group' => 'Force JS',
    );
  }

  /**
   * {@inheritdoc}
   */
  public function setUp() {
    parent::setUp('forcejs', 'forcejs_test');
  }

  /**
   * Tests that for anonymous users the has_js cookie is enforced / removed.
   */
  public function testForceJsAnonymousUsers() {
    $cookie_headers = array(
      'Cookie: has_js=1',
    );

    // Test standard settings.
    $this->drupalGet('<front>');
    $this->assertIdentical($this->drupalGetHeader('X-Test-Has-Js'), FALSE, 'The has_js cookie is not reported if it was missing on the request.');

    $this->drupalGet('<front>', array(), $cookie_headers);
    $this->assertIdentical($this->drupalGetHeader('X-Test-Has-Js'), FALSE, 'The has_js cookie is not reported even though it was present on the request if force_js is unconfigured.');

    // Test with forcejs enabled.
    variable_set('force_js', '1');

    $this->drupalGet('<front>');
    $this->assertIdentical($this->drupalGetHeader('X-Test-Has-Js'), '1', 'The has_js cookie is reported even though it was missing on the request if force_js is configured.');

    $this->drupalGet('<front>', array(), $cookie_headers);
    $this->assertIdentical($this->drupalGetHeader('X-Test-Has-Js'), '1', 'The has_js cookie is reported if it was present on the request.');

    // Test explicit "unset" setting.
    variable_set('force_js', 'unset');

    $this->drupalGet('<front>');
    $this->assertIdentical($this->drupalGetHeader('X-Test-Has-Js'), FALSE, 'The has_js cookie is not reported if it was missing on the request.');

    $this->drupalGet('<front>', array(), $cookie_headers);
    $this->assertIdentical($this->drupalGetHeader('X-Test-Has-Js'), FALSE, 'The has_js cookie is not reported even though it was present on the request if force_js is set to "unset".');

    variable_del('force_js');
  }

  /**
   * Tests that the has_js cookie is not modified for authenticated users.
   */
  public function testForceJsAuthenticatedUser() {
    $account = $this->drupalCreateUser();
    $this->drupalLogin($account);

    // Login sets the session cookie in the curl handle. If in addition a cookie
    // header is set using the headers parameter of drupalGet(), then the cookie
    // header will be duplicated. Therefore close the curl handle and manually
    // specify the session id on each request.
    $this->curlClose();
    $session_cookie = $this->session_name . '=' . $this->session_id;
    $nocookie_headers = array(
      'Cookie: ' . $session_cookie,
    );
    $cookie_headers = array(
      'Cookie: has_js=1;' . $session_cookie,
    );

    // Test standard settings.
    $this->drupalGet('<front>', array(), $nocookie_headers);
    $this->assertIdentical($this->drupalGetHeader('X-Test-Has-Js'), FALSE, 'The has_js cookie is not reported if it was missing on the request for authenticated users.');
    $this->assertIdentical($this->drupalGetHeader('X-Test-Uid'), $account->uid, 'The response was generated with the authenticated user.');

    $this->drupalGet('<front>', array(), $cookie_headers);
    $this->assertIdentical($this->drupalGetHeader('X-Test-Has-Js'), '1', 'The has_js cookie is reported if it was present on the request for authenticated users.');
    $this->assertIdentical($this->drupalGetHeader('X-Test-Uid'), $account->uid, 'The response was generated with the authenticated user.');

    // Test with forcejs enabled.
    variable_set('force_js', '1');

    $this->drupalGet('<front>', array(), $nocookie_headers);
    $this->assertIdentical($this->drupalGetHeader('X-Test-Has-Js'), '1', 'The has_js cookie is reported even though it was missing on the request for authenticated users if force_js is configured.');
    $this->assertIdentical($this->drupalGetHeader('X-Test-Uid'), $account->uid, 'The response was generated with the authenticated user.');

    $this->drupalGet('<front>', array(), $cookie_headers);
    $this->assertIdentical($this->drupalGetHeader('X-Test-Has-Js'), '1', 'The has_js cookie is reported if it was present on the request for authenticated users.');
    $this->assertIdentical($this->drupalGetHeader('X-Test-Uid'), $account->uid, 'The response was generated with the authenticated user.');

    // Test explicit "unset" setting.
    variable_set('force_js', 'unset');

    $this->drupalGet('<front>', array(), $nocookie_headers);
    $this->assertIdentical($this->drupalGetHeader('X-Test-Has-Js'), FALSE, 'The has_js cookie is not reported if it was missing on the request for authenticated users.');
    $this->assertIdentical($this->drupalGetHeader('X-Test-Uid'), $account->uid, 'The response was generated with the authenticated user.');

    $this->drupalGet('<front>', array(), $cookie_headers);
    $this->assertIdentical($this->drupalGetHeader('X-Test-Has-Js'), '1', 'The has_js cookie is reported if it was present on the request for authenticated users.');
    $this->assertIdentical($this->drupalGetHeader('X-Test-Uid'), $account->uid, 'The response was generated with the authenticated user.');

    variable_del('force_js');

    $this->drupalLogout();
  }

  /**
   * Tests the administrative interface.
   */
  public function testForceJsAdmin() {
    $account = $this->drupalCreateUser(array('administer site configuration'));
    $this->drupalLogin($account);

    $this->drupalGet('admin/config/development/performance');
    $this->assertFieldByName('force_js', 'unset', 'Enforce cookie value radio button is set to "Unset"');

    $this->assertNull(variable_get('force_js'));

    $edit = array(
      'force_js' => '1',
    );
    $this->drupalPost(NULL, $edit, 'Save configuration');
    $this->assertFieldByName('force_js', '1', 'Enforce cookie value radio button is set to "Enabled"');

    $this->assertEqual('1', variable_get('force_js'));

    $edit = array(
      'force_js' => 'unset',
    );
    $this->drupalPost(NULL, $edit, 'Save configuration');
    $this->assertFieldByName('force_js', 'unset', 'Enforce cookie value radio button is set to "Unset"');

    $this->assertEqual('unset', variable_get('force_js'));
  }

}
