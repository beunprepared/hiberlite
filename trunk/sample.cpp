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
	int a,b;
	vector<string> vs;
};

HIBERLITE_EXPORT_CLASS(MyClass)

sqlid_t objId;

void createDB()
{
	hiberlite::Database db("sample.db");
	db.registerBeanClass<MyClass>();	//register classes
	db.dropModel();		//drop all existing tables needed to store registered classes
	db.createModel();	//create those tables again with proper schema

	MyClass x;
	x.a=100; x.b=200;
	x.vs.push_back("Hello");
	x.vs.push_back("world");
	x.vs.push_back("!");

	hiberlite::bean_ptr<MyClass> p=db.copyBean(x);	//create a managed copy of the object in the database
	objId=p.get_id();
}

void loadData()
{
	hiberlite::Database db("sample.db");
	db.registerBeanClass<MyClass>();

	hiberlite::bean_ptr<MyClass> p = db.loadBean<MyClass>(objId);
	cout << "x.a=" << p->a << "\n";
	cout << "x.b=" << p->b << "\n";
	cout << "x.vs={";
	for(size_t i=0;i<p->vs.size();i++)
		i && cout << ", ", cout << p->vs[i];
	cout << "}\n";

}

int main()
{
	createDB();
	loadData();
	return 0;
}
