#include "Qt_CacheTool.h"

ISC_NSP_BEGIN

wchar_t Qt_CacheTool::__srv_cl_name[] = L"Qt.CacheTool";

// Properties

// Methods

d_string Qt_CacheTool::_ClassName(Database* __db, const d_bool& fullname)
{
   GEN_DECLARE_ARGS();
   wchar_t __mtd_name[] = L"%ClassName";

   __args_mgr.set_next(&fullname);

   d_string __res;
   __args_mgr.set_next_as_res(&__res);

   Critical_section cs(__db->get_cs_handle());
   __db->run_method(-1, __srv_cl_name, __mtd_name, __args_mgr);
   return __res;
}

d_int Qt_CacheTool::_IsA(Database* __db, const d_string& isclass)
{
   GEN_DECLARE_ARGS();
   wchar_t __mtd_name[] = L"%IsA";

   __args_mgr.set_next(&isclass);

   d_int __res;
   __args_mgr.set_next_as_res(&__res);

   Critical_section cs(__db->get_cs_handle());
   __db->run_method(-1, __srv_cl_name, __mtd_name, __args_mgr);
   return __res;
}

d_status Qt_CacheTool::Execute(const d_string& code)
{
   GEN_DECLARE_ARGS_AND_DB();
   wchar_t __mtd_name[] = L"Execute";

   __args_mgr.set_next(&code);

   d_status __res;
   __args_mgr.set_next_as_res(&__res);

   Critical_section cs(__db->get_cs_handle());
   __db->run_method(get_ref(), __srv_cl_name, __mtd_name, __args_mgr);
   return __res;
}

d_ref<d_bin_stream> Qt_CacheTool::ExportXML(const d_string& object)
{
   GEN_DECLARE_ARGS_AND_DB();
   wchar_t __mtd_name[] = L"ExportXML";

   __args_mgr.set_next(&object);

   d_ref<d_bin_stream> __res;
   __args_mgr.set_next_as_res(&__res);

   Critical_section cs(__db->get_cs_handle());
   __db->run_method(get_ref(), __srv_cl_name, __mtd_name, __args_mgr);
   return __res;
}

d_bool Qt_CacheTool::ImportXML(const d_string& xml)
{
   GEN_DECLARE_ARGS_AND_DB();
   wchar_t __mtd_name[] = L"ImportXML";

   __args_mgr.set_next(&xml);

   d_bool __res;
   __args_mgr.set_next_as_res(&__res);

   Critical_section cs(__db->get_cs_handle());
   __db->run_method(get_ref(), __srv_cl_name, __mtd_name, __args_mgr);
   return __res;
}

d_list Qt_CacheTool::ListObjects(const d_string& filter)
{
   GEN_DECLARE_ARGS_AND_DB();
   wchar_t __mtd_name[] = L"ListObjects";

   __args_mgr.set_next(&filter);

   d_list __res;
   __args_mgr.set_next_as_res(&__res);

   Critical_section cs(__db->get_cs_handle());
   __db->run_method(get_ref(), __srv_cl_name, __mtd_name, __args_mgr);
   return __res;
}

ISC_NSP_END
