require 'rubygems'

gem 'Mxx_ru', '>= 1.5.2'

require 'mxx_ru/cpp'

MxxRu::Cpp::composite_target( MxxRu::BUILD_ROOT ) {

		global_include_path( "." )
		default_runtime_mode( MxxRu::Cpp::RUNTIME_RELEASE )
		MxxRu::enable_show_brief

		required_prj "const_generator/prj.rb" 
}