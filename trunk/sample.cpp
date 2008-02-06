#include "hiberlite.h"

#include <string>
#include <vector>
#include <iostream>
using namespace std;

class MyClass{
	friend class hiberlite::access;
	template<class Archive>
	void hibernate(Archive & ar)
	{
		ar & HIBERLITE_NVP(a);
		ar & HIBERLITE_NVP(b);
		ar & HIBERLITE_NVP(vs);
	}
public:
	int a;
	double b;
	vector<string> vs;
};

HIBERLITE_EXPORT_CLASS(MyClass)

void createDB()
{
	hiberlite::Database db("sample.db");
	//register bean classes
	db.registerBeanClass<MyClass>();
	//drop all tables beans will use
	db.dropModel();
	//create those tables again with proper schema
	db.createModel();

	for(int i=0;i<5;i++) {
		MyClass x;
		x.a=i*10; x.b=1.0/(i+1);
		x.vs.push_back("Hello");
		x.vs.push_back("world");
		x.vs.push_back("!");

		hiberlite::bean_ptr<MyClass> p=db.copyBean(x);	//create a managed copy of the object in the database
	}
}

void loadData()
{
	hiberlite::Database db("sample.db");
	db.registerBeanClass<MyClass>();

	cout << "reading the DB\n";

	vector< hiberlite::bean_ptr<MyClass> > v=db.getAllBeans<MyClass>();

	cout << "found " << v.size() << " objects in the database\nhere's the first one:\n";

	cout << "x.a=" << v[0]->a << "\n";
	cout << "x.b=" << v[0]->b << "\n";
	cout << "x.vs={";
	for(size_t i=0;i<v[0]->vs.size();i++)
		i && cout << ", ", cout << v[0]->vs[i];
	cout << "}\n";
	cout << "now will destroy it.\n";
	v[0].destroy();
}

int main()
{
	createDB();
	loadData();
	loadData();
	return 0;
}
