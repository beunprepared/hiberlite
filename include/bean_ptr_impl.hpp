namespace hiberlite{

template<class C>
real_bean<C>::real_bean(const bean_key _key, C* _obj) : key(_key), obj(_obj), forgotten(false)
{}

template<class C>
real_bean<C>::~real_bean()
{
	if(forgotten)
		return;
	if(!obj)
		return;
	Database::dbUpdate(key, *obj);
	delete obj;
}

template<class C>
void real_bean<C>::destroy() {
	if(forgotten)
		return;
	Database::dbDelete(key, *obj);
	delete obj;
	forgotten=true;
	obj=NULL;
}

template<class C>
C* real_bean<C>::operator->()
{
	if(forgotten)
		return NULL;
	loadLazy();
	if(!obj)
		throw std::runtime_error("NULL pointer exception!");
	return obj;
}

template<class C>
C* real_bean<C>::get()
{
	if(forgotten)
		return NULL;
	loadLazy();
	if(!obj)
		throw std::runtime_error("NULL pointer exception!");
	return obj;
}

template<class C>
inline void real_bean<C>::loadLazy()
{
	if(!obj && key.id!=Database::NULL_ID)
		obj=Database::dbLoad<C>(key);
}

template<class C>
bean_ptr<C>::bean_ptr(bean_key k, real_bean<C>* rb) : shared_res< real_bean<C> >(rb), id(k.id)
{}

template<class C>
bean_ptr<C>::bean_ptr(const bean_ptr<C>& other) : shared_res< real_bean<C> >(other), id(other.id)
{
}

template<class C>
bean_ptr<C>& bean_ptr<C>::operator=(const bean_ptr<C>& other)
{
	shared_res< real_bean<C> >::operator=( other );
	id=other.id;
	return *this;
}

template<class C>
bean_ptr<C>::bean_ptr(bean_key k) : id(k.id)
{
	*this=Registry<C>::get(k);
}

template<class C>
bean_ptr<C>::bean_ptr() : id(Database::NULL_ID)
{
}

template<class C>
bean_ptr<C>::operator bool() const {
	return id!=Database::NULL_ID;
}

template<class C> template<class Archive>
void bean_ptr<C>::hibernate(Archive & ar)
{
	if(id!=Database::NULL_ID && ar.getConnection()!=(*this)->get_key().con)
		throw std::logic_error("saving the bean from different database");

	ar & hiberlite::sql_nvp< sqlid_t > ("id", id );
	if(ar.is_loading())
		*this=Registry<C>::get( bean_key(ar.getConnection(), id) );
}

template<class C>
C& bean_ptr<C>::operator*() {
	return *(shared_res< real_bean<C> >::get_object()->get());
}

} //namespace hiberlite
