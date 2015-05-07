#hiberlite::Database class description.

# Introduction #

**hiberlite::Database** represents a sqlite3 database.
It is noncopyable.

# Methods #
## Create database and open a sqlite3 file ##
  * **Database::Database(std::string filename)**
constructor that calls **Database::open(filename)** after initialization.
  * **Database::Database()**
default constructor, user must call
```
Database::open(...)
```
to open a database file.
  * **Database::open(std::string filename)** - open the sqlite3 database file.

## Database scheme operations ##
In the basic usage  scenario you let hiberlite to create database tables for you.
Each instance of hiberlite::Database has its own version of the database model - list of tables, needed to store user classes.

  * **Database::registerBeanClass**

&lt;Foo&gt;

()**- template method. Call it to notify that instances of the user-defined class "Foo" will be stored in this database. All tables, needed to store Foo objects, are added to the model.**

  * **Database::dropModel()** - clean the database.
This method executes "DROP IF EXISTS" SQL query for each table present in the model.

  * **Database::createModel()** - creates tables in the database.
Call this method to create all the tables (and columns) needed to store your classes.
You must call Database::registerBeanClass to make the Database aware of all the classes you plan to store in this Database.