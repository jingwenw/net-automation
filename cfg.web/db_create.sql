drop database DB_NAME_;
create database DB_NAME_;
use  DB_NAME_;
DELIMITER $$
CREATE DEFINER=`root`@`localhost` FUNCTION `isCategoryOf`(`cpid` INT, `ppidTo` INT) RETURNS tinyint(1)
    COMMENT 'To check if a given category pid is belongs to another'
    BEGIN
	DECLARE isa BOOLEAN;

	DECLARE parent BOOLEAN;
	DECLARE grand BOOLEAN;
	DECLARE grandGrand BOOLEAN;
	SET max_sp_recursion_depth=10;
	IF cpid = ppidTo THEN
	   	SET isa = true;
	ELSEIF cpid = 0 THEN
	        SET isa = false;
	ELSE
		SELECT ppid INTO parent FROM category WHERE pid = cpid;
		IF parent = ppidTo THEN
		   SET isa = true;
		ELSEIF parent = 0 THEN
		   SET isa = false;
		ELSE
		   SELECT ppid INTO grand FROM category WHERE pid = parent;
		   IF grand = ppidTo THEN
		      SET isa = true;
		   ELSEIF grand = 0 THEN
		      SET isa = false;
		   ELSE
    		      SELECT ppid INTO grandGrand FROM category	WHERE pid = grand;
		      IF grandGrand = ppidTo THEN
		      	 SET isa = true;
		      ELSE
		  	 SET isa = false;
		      END IF;
		   END IF;
	       END IF;
	END IF;
    RETURN (isa);
    END$$
    DELIMITER ;

grant all on DB_NAME_.* to 'jjwwang'@'localhost';

