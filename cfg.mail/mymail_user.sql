#drop table `virtual_users`;
#drop table `virtual_aliases`;
#drop table `virtual_domains`;

CREATE TABLE `virtual_domains` (
`id`  INT NOT NULL AUTO_INCREMENT,
`name` VARCHAR(50) NOT NULL,
PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `virtual_users` (
`id` INT NOT NULL AUTO_INCREMENT,
`domain_id` INT NOT NULL,
`password` VARCHAR(106) NOT NULL,
`email` VARCHAR(120) NOT NULL,
PRIMARY KEY (`id`),
UNIQUE KEY `email` (`email`),
FOREIGN KEY (domain_id) REFERENCES virtual_domains(id) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `virtual_aliases` (
`id` INT NOT NULL AUTO_INCREMENT,
`domain_id` INT NOT NULL,
`source` varchar(100) NOT NULL,
`destination` varchar(100) NOT NULL,
PRIMARY KEY (`id`),
FOREIGN KEY (domain_id) REFERENCES virtual_domains(id) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

INSERT INTO `virtual_domains`
(`id` ,`name`)
VALUES
('1', 'vancasoft.com'),
('2', 'localguider.com'),
('3', 'plateauclub.ca'),
('4', '7deals.ca'),
('5', 'wendu.ca');

INSERT INTO `virtual_users`
(`id`, `domain_id`, `password` , `email`)
VALUES
('1', '1', ENCRYPT('Van080430'), 'jwang@vancasoft.com'),
('2', '2', ENCRYPT('Loc080430'), 'jwang@localguider.com'),
('3', '3', ENCRYPT('Pla080430'), 'jwang@plateauclub.ca'),
('4', '4', ENCRYPT('Dea080430'), 'jwang@7deals.ca'),
('5', '5', ENCRYPT('Dea080430'), 'jwang@wendu.ca');


INSERT INTO `virtual_aliases`
(`id`, `domain_id`, `source`, `destination`)
VALUES
('1', '1', 'info@vancasoft.com', 'jwang@vancasoft.com'),
('2', '2', 'info@localguider.com', 'jwang@localguider.com'),
('3', '3', 'info@plateauclub.ca', 'jwang@plateauclub.ca'),
('4', '4', 'info@7deals.ca', 'jwang@7deals.ca'),
('5', '5', 'info@wendu.ca', 'jwang@wendu.ca');

