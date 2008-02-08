#include "hiberlite.h"

#include <string>
#include <vector>
#include <iostream>
using namespace std;

class Person{
	friend class hiberlite::access;
	template<class Archive>
	void hibernate(Archive & ar)
	{
		ar & HIBERLITE_NVP(name);
		ar & HIBERLITE_NVP(age);
		ar & HIBERLITE_NVP(bio);
	}
public:
	string name;
	int age;
	vector<string> bio;
};

HIBERLITE_EXPORT_CLASS(Person)

void createDB()
{
	hiberlite::Database db("sample.db");
	//register bean classes
	db.registerBeanClass<Person>();
	//drop all tables beans will use
	db.dropModel();
	//create those tables again with proper schema
	db.createModel();

	char* names[5]={"Stanley Marsh", "Kyle Broflovski", "Eric Theodore Cartman", "Kenneth McCormick", "Leopold Stotch"};

	for(int i=0;i<5;i++) {
		Person x;
		x.name=names[i%5];
		x.age=21+2*i;
		x.bio.push_back("Hello");
		x.bio.push_back("world");
		x.bio.push_back("!");

		hiberlite::bean_ptr<Person> p=db.copyBean(x);	//create a managed copy of the object
	}
}

void loadData()
{
	hiberlite::Database db("sample.db");
	db.registerBeanClass<Person>();

	cout << "reading the DB\n";

	vector< hiberlite::bean_ptr<Person> > v=db.getAllBeans<Person>();

	cout << "found " << v.size() << " persons in the database\nhere's the first one:\n";

	cout << "name=" << v[0]->name << "\n";
	cout << "age=" << v[0]->age << "\n";
	cout << "bio={";
	for(size_t i=0;i<v[0]->bio.size();i++)
		i && cout << ", ", cout << v[0]->bio[i];
	cout << "}\n";
	cout << v[0]->name << " will be deleted.\n\n";
	v[0].destroy();
}

int main()
{
	createDB();
	loadData();
	loadData();
	return 0;
}
