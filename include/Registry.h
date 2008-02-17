#ifndef REGISTRY_H_INCLUDED
#define REGISTRY_H_INCLUDED

namespace hiberlite{

class Database;

template<class C> class bean_ptr;

template<class C>
class Registry{
	protected:
		static std::map<bean_key,bean_ptr<C> > beans;

	public:
		static void put(const bean_key key, const bean_ptr<C> ptr);
		static bean_ptr<C> get(const bean_key key);
		static bool has(const bean_key key);

		static bean_ptr<C> createBeanPtr(bean_key key, C* obj);

		static void dying(bean_key key);
};

template<class C>
std::map< bean_key, bean_ptr<C> > Registry<C>::beans;

} //namespace hiberlite

#endif // REGISTRY_H_INCLUDED
