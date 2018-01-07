solution 'prs'
	configurations { 'dev' }

	location 'build'
	objdir 'build'
	targetdir 'build'

	if _ACTION == 'clean' then
		os.rmdir('build')
	end

	language 'C'
	files { 'src/prs.h' }
	includedirs { 'src' }

	configuration 'not vs*'
		buildoptions { '-ansi', '-pedantic', '-Wall', '-Wextra', '-Werror' }

	project 'tests'
		kind 'ConsoleApp'
		files { 'src/tests/*.*' }

	local samples = os.matchfiles('src/samples/*.c')
	for i, file in ipairs(samples) do
		local name = path.getbasename(path.getname(file))
		project(name)
			kind 'ConsoleApp'
			files { file }
	end
