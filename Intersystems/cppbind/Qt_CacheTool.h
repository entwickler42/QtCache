#ifndef __Qt_CacheTool
#define __Qt_CacheTool

#include <cppbind.h>

ISC_NSP_BEGIN

class Registered_t;
class Qt_CacheTool;

class Qt_CacheTool : public Registered_t {
   friend class d_ref<Qt_CacheTool>;

protected:
   Qt_CacheTool() {}

private:
   static wchar_t __srv_cl_name[13];

   Qt_CacheTool(Database* db_arg, int oref, const wchar_t* cl_name)
   { init(db_arg, oref, cl_name); }

public:
   virtual ~Qt_CacheTool() {}

   static d_ref<Qt_CacheTool> openref(t_istream& in, Database* db_arg)
   {
      int oref; cl_name_t tname;
      abs_d_ref::get_oref_n_name(in, &oref, tname);
      return openref(db_arg, oref, tname);
   }

   static d_ref<Qt_CacheTool> openref(Database* db_arg, int oref, const_name_t cl_name)
   {
      if (oref == 0)
         return d_ref<Qt_CacheTool>();

      Obj_t *obj; int *num_refs;
      if (db_arg->get_proxies_info()->get_proxy_info(oref, typeid(Qt_CacheTool), &obj, &num_refs))
         return d_ref<Qt_CacheTool>(dynamic_cast<Qt_CacheTool*>(obj), num_refs);
      else
         return d_ref<Qt_CacheTool>(new Qt_CacheTool(db_arg, oref, cl_name));
   }

   static d_ref<Qt_CacheTool> create_new(Database* db_arg, const_str_t init_val = 0, Db_err* err = 0)
   {
      Critical_section cs(db_arg->get_cs_handle());
      return openref(db_arg->make_obj(__srv_cl_name, init_val, err), db_arg);
   }

   // Properties

   // Methods

   static d_string _ClassName(Database* __db, const d_bool& fullname);

   static d_int _IsA(Database* __db, const d_string& isclass);

   virtual d_status Execute(const d_string& code);

   virtual d_ref<d_bin_stream> ExportXML(const d_string& object);

   virtual d_bool ImportXML(const d_string& xml);

   virtual d_list ListObjects(const d_string& filter);

   // Queries
};

ISC_NSP_END

#endif
