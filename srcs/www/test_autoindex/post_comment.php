<?php
require('Persistence.php');

$db = new Persistence();
$added = $db->add_comment($_POST);
print_r($_POST);
if($added) {
  header( 'Location: clear.php' );
  exit();
}
else {
  header( 'Location: ./html/index.php?error=Your comment was not posted due to errors in your form submission' );
  exit();
}
?>
