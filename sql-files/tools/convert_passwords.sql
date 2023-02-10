# Convert passwords to MD5 hashes

ALTER TABLE `login` ADD `salt`  varchar(32) NOT NULL default '';
UPDATE `login` SET `salt`=MD5(RAND());
UPDATE `login` SET `user_pass`=MD5(CONCAT(`user_pass`, `salt`));
