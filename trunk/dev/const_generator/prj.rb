require 'mxx_ru/cpp'

MxxRu::Cpp::exe_target {

	target( "const_generator" )

	cpp_source( "cmd_parser.cpp" )
	cpp_source( "cfg.cpp" )
	cpp_source( "generator.cpp" )
	cpp_source( "main.cpp" )
}
