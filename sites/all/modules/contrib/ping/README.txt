Ping is designed to respond to up checks or "pings". The module ensures that 
Drupal is basically functioning as it retreieves data from the database. The
response message is configurable via an admin form.  The appropriate headers
are set to ensure the response isn't cached.

Ping will not be backported to Drupal 6 as the namespace is taken by a core 
module with very different functionality.  

** This module is _not_ the Drupal 7 successor or a port of the ping module 
from Drupal 6 core. **

Ping takes a conservative approach to permissions, you need to explictly grant 
access to the response page (admin/people/permissions#module-ping). 

You may also need to change the response message.  The default message is "OK",
but you can change this on the admin settings page (admin/config/system/ping).  

The response page should be available at http://example.com/ping
