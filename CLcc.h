// Machine generated IDispatch wrapper class(es) created with Add Class from Typelib Wizard

#import "C:\\Projects\\835\\Lcc Installation\\Programs\\Lcc.tlb" no_namespace
// CLcc wrapper class

class CLcc : public COleDispatchDriver
{
public:
	CLcc(){} // Calls COleDispatchDriver default constructor
	CLcc(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CLcc(const CLcc& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
public:

	// Operations
public:


	// ILcc methods
public:
	long Synch()
	{
		long result;
		InvokeHelper(0x1, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	long Inquire(long InquireType, VARIANT * Status)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_PVARIANT ;
		InvokeHelper(0x2, DISPATCH_METHOD, VT_I4, (void*)&result, parms, InquireType, Status);
		return result;
	}
	long CSOpen(long ClassID, long ProcID)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x3, DISPATCH_METHOD, VT_I4, (void*)&result, parms, ClassID, ProcID);
		return result;
	}
	long CSStart()
	{
		long result;
		InvokeHelper(0x4, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	long CSStop(long Mode)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x5, DISPATCH_METHOD, VT_I4, (void*)&result, parms, Mode);
		return result;
	}
	long CSClose()
	{
		long result;
		InvokeHelper(0x6, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	long CSGetStatus()
	{
		long result;
		InvokeHelper(0x7, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	long GetProcLast(long ClassID)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x8, DISPATCH_METHOD, VT_I4, (void*)&result, parms, ClassID);
		return result;
	}
	long GetProcName(long ClassID, long ProcID, VARIANT * Name)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_PVARIANT ;
		InvokeHelper(0x9, DISPATCH_METHOD, VT_I4, (void*)&result, parms, ClassID, ProcID, Name);
		return result;
	}
	long GetProcAttr(long ClassID, long ProcID, VARIANT * Attribute)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_PVARIANT ;
		InvokeHelper(0xa, DISPATCH_METHOD, VT_I4, (void*)&result, parms, ClassID, ProcID, Attribute);
		return result;
	}
	long GetParamLast(long ClassID, long ProcID, VARIANT * ParamID)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_PVARIANT ;
		InvokeHelper(0xb, DISPATCH_METHOD, VT_I4, (void*)&result, parms, ClassID, ProcID, ParamID);
		return result;
	}
	long GetParamName(long ClassID, long ProcID, long ParamID, VARIANT * Name)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 VTS_PVARIANT ;
		InvokeHelper(0xc, DISPATCH_METHOD, VT_I4, (void*)&result, parms, ClassID, ProcID, ParamID, Name);
		return result;
	}
	long GetParamType(long ClassID, long ProcID, long ParamID, VARIANT * ParamType)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 VTS_PVARIANT ;
		InvokeHelper(0xd, DISPATCH_METHOD, VT_I4, (void*)&result, parms, ClassID, ProcID, ParamID, ParamType);
		return result;
	}
	long GetParamMaxVal(long ClassID, long ProcID, long ParamID, VARIANT * Limit)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 VTS_PVARIANT ;
		InvokeHelper(0xe, DISPATCH_METHOD, VT_I4, (void*)&result, parms, ClassID, ProcID, ParamID, Limit);
		return result;
	}
	long GetParamMinVal(long ClassID, long ProcID, long ParamID, VARIANT * Limit)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 VTS_PVARIANT ;
		InvokeHelper(0xf, DISPATCH_METHOD, VT_I4, (void*)&result, parms, ClassID, ProcID, ParamID, Limit);
		return result;
	}
	long GetParamNomVal(long ClassID, long ProcID, long ParamID, VARIANT * Value)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 VTS_PVARIANT ;
		InvokeHelper(0x10, DISPATCH_METHOD, VT_I4, (void*)&result, parms, ClassID, ProcID, ParamID, Value);
		return result;
	}
	long CSGetReagent(long InjectorID, VARIANT * Volume)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_PVARIANT ;
		InvokeHelper(0x11, DISPATCH_METHOD, VT_I4, (void*)&result, parms, InjectorID, Volume);
		return result;
	}
	long CSGetTimeLeft(VARIANT * Time)
	{
		long result;
		static BYTE parms[] = VTS_PVARIANT ;
		InvokeHelper(0x12, DISPATCH_METHOD, VT_I4, (void*)&result, parms, Time);
		return result;
	}
	long CSGetData(long Array, long Index, VARIANT * Value)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_PVARIANT ;
		InvokeHelper(0x13, DISPATCH_METHOD, VT_I4, (void*)&result, parms, Array, Index, Value);
		return result;
	}
	long CSSetParam(long ParamID, VARIANT * Value)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_PVARIANT ;
		InvokeHelper(0x14, DISPATCH_METHOD, VT_I4, (void*)&result, parms, ParamID, Value);
		return result;
	}
	long ShowWindow()
	{
		long result;
		InvokeHelper(0x15, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	long SetupCommunication()
	{
		long result;
		InvokeHelper(0x16, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}

	// ILcc properties
public:

};
