namespace hiberlite{

template<class C>
void Registry<C>::put(const bean_key key, const bean_ptr<C> ptr)
{
	if(beans.find(key)!=beans.end())
		throw std::logic_error("bean already exists");
	beans[key]=ptr;
}

template<class C>
bean_ptr<C> Registry<C>::createBeanPtr(bean_key key, C* obj)
{
	if(key.id==Database::NULL_ID)
		return bean_ptr<C>(key,NULL);

	real_bean<C>* rb=new real_bean<C>(key,obj);
	bean_ptr<C> ans(key, rb);
	put(key, ans);
	return ans;
}

template<class C>
bean_ptr<C> Registry<C>::get(const bean_key key)
{
	typename std::map<bean_key,bean_ptr<C> >::iterator it;
	it=beans.find(key);
	if(it==beans.end())
		return createBeanPtr(key,NULL);
	else
		return it->second;
}

template<class C>
bool Registry<C>::has(const bean_key key)
{
	return beans.find(key)!=beans.end();
}

template<class C>
void Registry<C>::dying(bean_key key){
/*TODO	if(!has(key))
		throw std::logic_error("unknown key death reported");
	if(get(key).get_ref_count()==2) // the only users are the Registry itself and the method caller, which is being destroyed now
		beans.erase(key);*/
}

} // namespace hiberlite
