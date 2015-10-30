#include "Qt_CacheTool.h"

ISC_NSP_BEGIN

wchar_t Qt_CacheTool::__srv_cl_name[] = L"Qt.CacheTool";

// Properties

void Qt_CacheTool::setLastStatus(const d_status& prop_val)
{
   GEN_DECLARE_ARGS_AND_DB();
   wchar_t __prop_name[] = L"LastStatus";

   __args_mgr.set_next(&prop_val);
   Critical_section cs(__db->get_cs_handle());
   __db->set_property(this, get_cl_name(), __prop_name, __args_mgr);
}

d_status Qt_CacheTool::getLastStatus() const
{
   GEN_DECLARE_ARGS_AND_DB();
   wchar_t __prop_name[] = L"LastStatus";

   d_status __res;
   __args_mgr.set_next_as_res(&__res);
   Critical_section cs(__db->get_cs_handle());
   __db->get_property(this, get_cl_name(), __prop_name, __args_mgr);
   return __res;
}

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

d_status Qt_CacheTool::Execute(const d_string& uci, const d_string& code)
{
   GEN_DECLARE_ARGS_AND_DB();
   wchar_t __mtd_name[] = L"Execute";

   __args_mgr.set_next(&uci);
   __args_mgr.set_next(&code);

   d_status __res;
   __args_mgr.set_next_as_res(&__res);

   Critical_section cs(__db->get_cs_handle());
   __db->run_method(get_ref(), __srv_cl_name, __mtd_name, __args_mgr);
   return __res;
}

d_ref<d_file_bin_stream> Qt_CacheTool::ExportXML(const d_string& uci, const d_string& object)
{
   GEN_DECLARE_ARGS_AND_DB();
   wchar_t __mtd_name[] = L"ExportXML";

   __args_mgr.set_next(&uci);
   __args_mgr.set_next(&object);

   d_ref<d_file_bin_stream> __res;
   __args_mgr.set_next_as_res(&__res);

   Critical_section cs(__db->get_cs_handle());
   __db->run_method(get_ref(), __srv_cl_name, __mtd_name, __args_mgr);
   return __res;
}

d_status Qt_CacheTool::GetLastStatus()
{
   GEN_DECLARE_ARGS_AND_DB();
   wchar_t __mtd_name[] = L"GetLastStatus";

   d_status __res;
   __args_mgr.set_next_as_res(&__res);

   Critical_section cs(__db->get_cs_handle());
   __db->run_method(get_ref(), __srv_cl_name, __mtd_name, __args_mgr);
   return __res;
}

d_status Qt_CacheTool::ImportXML(const d_string& uci, const d_string& xml)
{
   GEN_DECLARE_ARGS_AND_DB();
   wchar_t __mtd_name[] = L"ImportXML";

   __args_mgr.set_next(&uci);
   __args_mgr.set_next(&xml);

   d_status __res;
   __args_mgr.set_next_as_res(&__res);

   Critical_section cs(__db->get_cs_handle());
   __db->run_method(get_ref(), __srv_cl_name, __mtd_name, __args_mgr);
   return __res;
}

d_status Qt_CacheTool::LastStatusIsValid(Database* __db, const d_string& value)
{
   GEN_DECLARE_ARGS();
   wchar_t __mtd_name[] = L"LastStatusIsValid";

   __args_mgr.set_next(&value);

   d_status __res;
   __args_mgr.set_next_as_res(&__res);

   Critical_section cs(__db->get_cs_handle());
   __db->run_method(-1, __srv_cl_name, __mtd_name, __args_mgr);
   return __res;
}

d_string Qt_CacheTool::LastStatusLogicalToOdbc(Database* __db)
{
   GEN_DECLARE_ARGS();
   wchar_t __mtd_name[] = L"LastStatusLogicalToOdbc";

   d_string __res;
   __args_mgr.set_next_as_res(&__res);

   Critical_section cs(__db->get_cs_handle());
   __db->run_method(-1, __srv_cl_name, __mtd_name, __args_mgr);
   return __res;
}

d_string Qt_CacheTool::LastStatusLogicalToOdbc(Database* __db, const d_string& _val)
{
   GEN_DECLARE_ARGS();
   wchar_t __mtd_name[] = L"LastStatusLogicalToOdbc";

   __args_mgr.set_next(&_val);

   d_string __res;
   __args_mgr.set_next_as_res(&__res);

   Critical_section cs(__db->get_cs_handle());
   __db->run_method(-1, __srv_cl_name, __mtd_name, __args_mgr);
   return __res;
}

d_list Qt_CacheTool::ListNamespaces()
{
   GEN_DECLARE_ARGS_AND_DB();
   wchar_t __mtd_name[] = L"ListNamespaces";

   d_list __res;
   __args_mgr.set_next_as_res(&__res);

   Critical_section cs(__db->get_cs_handle());
   __db->run_method(get_ref(), __srv_cl_name, __mtd_name, __args_mgr);
   return __res;
}

d_ref<d_file_char_stream> Qt_CacheTool::ListObjects(const d_string& uci, const d_string& filter)
{
   GEN_DECLARE_ARGS_AND_DB();
   wchar_t __mtd_name[] = L"ListObjects";

   __args_mgr.set_next(&uci);
   __args_mgr.set_next(&filter);

   d_ref<d_file_char_stream> __res;
   __args_mgr.set_next_as_res(&__res);

   Critical_section cs(__db->get_cs_handle());
   __db->run_method(get_ref(), __srv_cl_name, __mtd_name, __args_mgr);
   return __res;
}

ISC_NSP_END
