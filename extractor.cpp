lua_State* L = luaL_newstate();
	luaL_openlibs(L);

	std::string input;
	while (std::getline(std::cin, input))
	{
		if (!luaL_loadstring(L, input.c_str()))
		{
			LClosure* newFunc = &clvalue(L->top - 1)->l;
			Proto* p = newFunc->p;

			std::cout << "Proto information:\n\n";
			std::cout << "sizecode = " << p->sizecode << std::endl;
			std::cout << "sizek = " << p->sizek << std::endl;
			std::cout << "sizep = " << p->sizep << std::endl;
			std::cout << "sizeupvalues = " << p->sizeupvalues << std::endl;
			std::cout << "sizelocvars = " << p->sizelocvars << std::endl;
			std::cout << "maxstacksize = " << static_cast<int>(p->maxstacksize) << std::endl; // lua makes these unsigned char so I need to make them p r o p e r
			std::cout << "numparams = " << static_cast<int>(p->numparams) << std::endl;
			std::cout << "nups = " << static_cast<int>(p->nups) << std::endl;
			std::cout << "is_vararg = " << static_cast<int>(p->is_vararg) << std::endl;

			std::cout << "\nConstants:\n\n";
			for (int i = 0; i < p->sizek; i++)
			{
				TValue* val = &p->k[i];
				switch (ttype(val))
				{
				case LUA_TNIL: {
					printf_s("%d) nil - 0\n", i);
					break;
				}
				case LUA_TNUMBER: {
					printf_s("%d) number - %f\n", i, val->value.n);
					break;
				}
				case LUA_TBOOLEAN: {
					printf_s("%d) boolean - %s\n", i, val->value.b ? "True" : "False");
					break;
				}
				case LUA_TSTRING: {
					printf_s("%d) string - %s\n", i, svalue(val));
					break;
				}
				default: {
					std::cout << "Unknown type: " << lua_type(L, val->tt) << std::endl;
				}
				}
			}
			std::cout << "\nInstructions:\n\n";
			for (int i = 0; i < p->sizecode; i++)
			{
				Instruction curr = p->code[i];
				switch (GET_OPCODE(curr))
				{
					case OP_GETGLOBAL: {
						printf_s("OP_GETGLOBAL	-	A = %d, Bx = %d\n", GETARG_A(curr), GETARG_Bx(curr));
						break;
					}
					case OP_SETGLOBAL: {
						printf_s("OP_SETGLOBAL	-	A = %d, Bx = %d\n", GETARG_A(curr), GETARG_Bx(curr));
						break;
					}
					case OP_MOVE: {
						printf_s("OP_MOVE	-	A = %d, B = %d\n", GETARG_A(curr), GETARG_B(curr));
						break;
					}
					case OP_LOADK: {
						printf_s("OP_LOADK	-	A = %d, Bx = %d\n", GETARG_A(curr), GETARG_Bx(curr));
						break;
					}
					case OP_LOADBOOL: {
						printf_s("OP_LOADBOOL	-	A = %d, B = %d, C = %d\n", GETARG_A(curr), GETARG_B(curr), GETARG_C(curr));
						break;
					}
					case OP_LOADNIL: {
						printf_s("OP_LOADNIL	-	A = %d, B = %d\n", GETARG_A(curr), GETARG_B(curr));
						break;
					}
					case OP_CALL: {
						printf_s("OP_CALL		-	A = %d, B = %d, C = %d\n", GETARG_A(curr), GETARG_B(curr), GETARG_C(curr));
						break;
					}
					case OP_RETURN: {
						printf_s("OP_RETURN	-	A = %d, B = %d\n", GETARG_A(curr), GETARG_B(curr));
						break;
					}
					default: {
						std::cout << "Unknown opcode: " << GET_OPCODE(curr) << std::endl;
					}
				}
			}
		}
	}
