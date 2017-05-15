CREATE USER 'mymailuser'@'localhost' IDENTIFIED BY '720913';
CREATE USER 'mymailuser'@'%' IDENTIFIED BY '720913';

GRANT ALL ON mymaildb.* TO 'mymailuser'@'localhost';
GRANT ALL ON mymaildb.* TO 'mymailuser'@'%';

FLUSH PRIVILEGES;
