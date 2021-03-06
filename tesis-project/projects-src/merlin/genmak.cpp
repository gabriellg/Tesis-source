//genmak: Generación de .mak para proyectos de merlin.

#include "genmak.inl"

#include "compilador.inl"
#include "fichsrc.inl"

#include "asrtbas.h"
#include "arrcad.h"
#include "array.h"
#include "strbas.h"
#include "memory.h"
#include "nomfich.h"

#include "filetxt.hpp"

struct prv_program_t
{
	struct fichsrc_t *fileMain;

	ArrTipo(fichsrc_t) *filesSource;
	ArrTipo(fichsrc_t) *filesIncludes;
	ArrString *includes_de_terceros;
};

struct genmak_t
{
	char *nameProject;
	ArrTipo(fichsrc_t) *filesIncludeProject;
	ArrTipo(prv_program_t) *programs;
};

static const char *PRV_DIR_OBJS_MAKE = "./objs";
static const char *PRV_DIR_SOURCE_MAKE = "./src";
static const char *PRV_DIR_BIN_MAKE = "./bin";

static const char *PRV_EXTENSION_MAK = "mk";

static const char *PRV_ONLY_DIR_SOURCE_MAKE = "src";
static const char *PRV_ONLY_DIR_OBJS_MAKE = "objs";
static const char *PRV_ONLY_DIR_BIN_MAKE = "bin";
static const char *PRV_MAKEFILE = "makefile";

static const char *PRV_MACRO_OBJS = "OBJS";
static const char *PRV_MACRO_COMPILER = "CC";
static const char *PRV_MACRO_LINKER = "LINKER";

//-----------------------------------------------------------------------

static char *prv_referenceMacro(const char *nameMacro)
{
	return strbas_printf("$(%s)", nameMacro);
}

//-----------------------------------------------------------------------

static void prv_appendLineComment(ArrString *codeMak)
{
	arrcad_Anyadir(codeMak, "################################################################################");
}

//-----------------------------------------------------------------------

static void prv_appendComment(ArrString *codeMak, char **txtComment)
{
	char *comment;

	assert_no_null(txtComment);
	assert_no_null(*txtComment);

	comment = strbas_printf("# %s", *txtComment);

	arrcad_AnyadirDestruyendoCadena(codeMak, &comment);
	strbas_destroy(txtComment);
}

//-----------------------------------------------------------------------

static void prv_appendCommentConst(ArrString *codeMak, const char *txtComment)
{
	char *comment;

	comment = strbas_printf("# %s", txtComment);
	arrcad_AnyadirDestruyendoCadena(codeMak, &comment);
}

//-----------------------------------------------------------------------

static void prv_appendTargetDepends(
					const char *txtTarget,
					const char *txtDepends,
					ArrString **commands_opc,
					ArrString *codeMak)
{
	char *targetDepends;

	targetDepends = strbas_printf("%s: %s", txtTarget, txtDepends);
	arrcad_AnyadirDestruyendoCadena(codeMak, &targetDepends);

	if (commands_opc != NULL)
	{
		unsigned long i, num;

		num = arrcad_numElementos(*commands_opc);

		for (i = 0; i < num; i++)
		{
			const char *command;
			char *commandTarget;

			command = arrcad_Get(*commands_opc, i);
			commandTarget = strbas_printf("\t%s", command);
			arrcad_AnyadirDestruyendoCadena(codeMak, &commandTarget);
		}

		arrcad_Destruye(commands_opc);
	}
}

//-----------------------------------------------------------------------

static void prv_appendMacroValue(const char *nameMacro, const char *value, ArrString *codeMak)
{
	char *defineMacro;

	defineMacro = strbas_printf("%s = %s", nameMacro, value);
	arrcad_AnyadirDestruyendoCadena(codeMak, &defineMacro);
}

//-----------------------------------------------------------------------

static void prv_appendMacroMultiValues(const char *nameMacro, const ArrString *values, ArrString *codeMak)
{
	char *defineMacro;
	unsigned long i, num;

	defineMacro = strbas_printf("%s =\\", nameMacro);
	arrcad_AnyadirDestruyendoCadena(codeMak, &defineMacro);
	num = arrcad_numElementos(values);

	for (i = 0; i < num; i++)
	{
		char *value;

		value = arrcad_GetCopia(values, i);

		if (i != num - 1)
			strbas_concatenate(&value," \\");

		arrcad_AnyadirDestruyendoCadena(codeMak, &value);
	}
}

//-----------------------------------------------------------------------

static void prv_appendHeader(const char *nameProject, ArrString *codeMak)
{
	char *txtHeader;

	txtHeader = strbas_printf("Automatically-generated file to project %s", nameProject);

	prv_appendLineComment(codeMak);
	prv_appendComment(codeMak, &txtHeader);
	prv_appendLineComment(codeMak);

	arrcad_Anyadir(codeMak, "");
}

//-----------------------------------------------------------------------

static void prv_appendInclude(const char *nameInclude, ArrString *codeMak)
{
	char *include;

	include = strbas_printf("-include %s", nameInclude);
	arrcad_AnyadirDestruyendoCadena(codeMak, &include);
}

//-----------------------------------------------------------------------

static void prv_appendIncludes(const ArrString *includesMak, ArrString *codeMak)
{
	unsigned long i, num;

	num = arrcad_numElementos(includesMak);

	for (i = 0; i < num; i++)
	{
		const char *nameInclude;
		char *nameWithPath;

		nameInclude = arrcad_Get(includesMak, i);
		nameWithPath = nomfich_append_a_directorio(".",nameInclude);
		prv_appendInclude(nameWithPath, codeMak);
		strbas_destroy(&nameWithPath);
	}

	arrcad_Anyadir(codeMak, "");
}

//-----------------------------------------------------------------------

static struct prv_program_t *prv_createProgram(
							struct fichsrc_t **fileMain,
							ArrTipo(fichsrc_t) **filesSource,
							ArrTipo(fichsrc_t) **filesIncludes,
							ArrString **includes_de_terceros)
{
	struct prv_program_t *program;

	program = MALLOC(struct prv_program_t);

	program->fileMain = ASSIGN_PP_NO_NULL(fileMain, struct fichsrc_t);

	program->filesSource = ASSIGN_PP_NO_NULL(filesSource, ArrTipo(fichsrc_t));
	program->filesIncludes = ASSIGN_PP_NO_NULL(filesIncludes, ArrTipo(fichsrc_t));
	program->includes_de_terceros = ASSIGN_PP_NO_NULL(includes_de_terceros, ArrString);

	return program;
}

//-----------------------------------------------------------------------

static void prv_destroyProgram(struct prv_program_t **program)
{
	assert_no_null(program);
	assert_no_null(*program);

	fichsrc_destruye(&(*program)->fileMain);
	array_Destruye(fichsrc_t, &(*program)->filesSource, fichsrc_destruye);
	array_Destruye(fichsrc_t, &(*program)->filesIncludes, fichsrc_destruye);
	arrcad_Destruye(&(*program)->includes_de_terceros);

	FREE_T(program, struct prv_program_t);
}

//-----------------------------------------------------------------------

static ArrString *prv_filesObjs(const ArrTipo(fichsrc_t) *filesSource, const struct compilador_t *compilador)
{
	ArrString *filesObjs;
	unsigned long i, num;

	num = array_numElementos(fichsrc_t, filesSource);

	filesObjs = arrcad_CreaString(num);

	for (i = 0; i < num; i++)
	{
		const struct fichsrc_t *fileSource;
		char *fileBaseObj, *fileObj;

		fileSource = array_Get(fichsrc_t, filesSource, i);
		fileBaseObj = compilador_nombre_obj(compilador, fileSource);

		fileObj = nomfich_append_a_directorio(PRV_DIR_OBJS_MAKE, fileBaseObj);
		arrcad_SetDestruyendoCadena(filesObjs, i, &fileObj);
		strbas_destroy(&fileBaseObj);
	}

	return filesObjs;
}

//-----------------------------------------------------------------------

static void prv_appendDefineMacroObjs(
				const ArrTipo(fichsrc_t) *filesSource,
				const struct compilador_t *compilador,
				ArrString *codeMak)
{
	ArrString *fileObjs;

	fileObjs = prv_filesObjs(filesSource, compilador);
	prv_appendMacroMultiValues(PRV_MACRO_OBJS, fileObjs, codeMak);
	arrcad_Anyadir(codeMak, "");
	arrcad_Destruye(&fileObjs);
}

//-----------------------------------------------------------------------

static void prv_appendEcho(const char *txtEcho, ArrString *codeMak)
{
	char *echo;

	echo = strbas_printf("@echo '%s'", txtEcho);
	arrcad_AnyadirDestruyendoCadena(codeMak, &echo);
}

//-----------------------------------------------------------------------

static ArrString *prv_commandLinker(
							const struct compilador_t *compiler,
							const char *varFlagsLinker,
							ArrString *codeMak)
{
	ArrString *commandCreateExecutable;
	char *commandLinker;

	commandLinker = strbas_printf(
							"$(%s) $(OBJS) $(%s) -o\"$@\"",
							PRV_MACRO_LINKER, varFlagsLinker);

	commandCreateExecutable = arrcad_CreaString(0);

	prv_appendEcho("Linkando: $@", commandCreateExecutable);
	arrcad_AnyadirDestruyendoCadena(commandCreateExecutable, &commandLinker);
	prv_appendEcho("Fin linkando: $@", commandCreateExecutable);
	prv_appendEcho(" ", commandCreateExecutable);

	return commandCreateExecutable;
}

//-----------------------------------------------------------------------

static void prv_appendCreateExecutable(
							const struct compilador_t *compiler,
							const char *varFlagsLinker,
							const char *nameProgram,
							ArrString *codeMak)
{
	char *referenceObjs, *targetNameProgram;
	ArrString *commandCreateExecutable;

	commandCreateExecutable = prv_commandLinker(compiler, varFlagsLinker, codeMak);

	referenceObjs = prv_referenceMacro(PRV_MACRO_OBJS);
	targetNameProgram = nomfich_append_a_directorio(PRV_DIR_BIN_MAKE, nameProgram);

	prv_appendTargetDepends(targetNameProgram, referenceObjs, &commandCreateExecutable, codeMak);
	arrcad_Anyadir(codeMak, "");

	strbas_destroy(&referenceObjs);
	strbas_destroy(&targetNameProgram);
}

//-----------------------------------------------------------------------

static ArrString *prv_commandCompile(
							const struct compilador_t *compiler,
							const char *varFlagsCompiler)
{
	ArrString *commandCompileSource;
	char *commandCompiler;

	commandCompiler = strbas_printf("$(%s) $(%s) -o\"./$@\" \"./$<\"", PRV_MACRO_COMPILER, varFlagsCompiler);

	commandCompileSource = arrcad_CreaString(0);

	prv_appendEcho("Compilando $< ...", commandCompileSource);
	arrcad_AnyadirDestruyendoCadena(commandCompileSource, &commandCompiler);
	prv_appendEcho("Fin Compilador: $<", commandCompileSource);
	prv_appendEcho(" ", commandCompileSource);

	return commandCompileSource;
}

//-----------------------------------------------------------------------

static char *prv_nameFileInSrc(const struct fichsrc_t *fileSource)
{
	char *strSource, *strFileSource;

	strSource = fichsrc_nombre_base_con_extension(fileSource);

	strFileSource = nomfich_append_a_directorio(PRV_DIR_SOURCE_MAKE, strSource);

	strbas_destroy(&strSource);

	return strFileSource;
}

//-----------------------------------------------------------------------

static void prv_appendCompileSource(
							const struct compilador_t *compiler,
							const struct fichsrc_t *fileSource,
							const char *varFlagsCompiler,
							ArrString *codeMak)
{
	ArrString *commandCompiler;
	char *strObj, *strFileSource, *strFileObj;

	strFileSource = prv_nameFileInSrc(fileSource);

	strObj = compilador_nombre_obj(compiler, fileSource);
	strFileObj = nomfich_append_a_directorio(PRV_DIR_OBJS_MAKE, strObj);

	commandCompiler = prv_commandCompile(compiler, varFlagsCompiler);

	prv_appendTargetDepends(strFileObj, strFileSource, &commandCompiler, codeMak);

	strbas_destroy(&strFileSource);
	strbas_destroy(&strFileObj);
	strbas_destroy(&strObj);
}

//-----------------------------------------------------------------------

static void prv_appendCompileSources(
							const ArrTipo(fichsrc_t) *filesSource,
							const char *varFlagsCompiler,
							const struct compilador_t *compiler,
							ArrString *codeMak)
{
	unsigned long i, num;

	num = array_numElementos(fichsrc_t, filesSource);

	for (i = 0; i < num; i++)
	{
		const struct fichsrc_t *fileSource;

		fileSource = array_Get(fichsrc_t, filesSource, i);
		prv_appendCompileSource(compiler, fileSource, varFlagsCompiler, codeMak);
		arrcad_Anyadir(codeMak, "");
	}
}

//-----------------------------------------------------------------------

static char *prv_appendDefineVarFlags(
						const char *prefixNameVar,
						const char *nameSubproject,
						const char *flags,
						ArrString *codeMak)
{
	char *nameVar;

	nameVar = strbas_printf("%s_%s", prefixNameVar, nameSubproject);
	strbas_toUppercase(nameVar);

	prv_appendMacroValue(nameVar, flags, codeMak);
	arrcad_Anyadir(codeMak, "");

	return nameVar;
}

//-----------------------------------------------------------------------

static char *prv_appendDefineCompilerFlags(
							const struct compilador_t *compiler,
							const char *nameSubproject,
							const ArrString *includes_de_terceros,
							const ArrString *ficherosInclude,
							ArrString *codeMak)
{
	char *nameVarMacroFlagsCompiler, *flagsCompiler;

	flagsCompiler = compilador_lineCommandMake(compiler, includes_de_terceros, ficherosInclude);

	nameVarMacroFlagsCompiler = prv_appendDefineVarFlags("FLAGS_COMPILER", nameSubproject, flagsCompiler, codeMak);

	strbas_destroy(&flagsCompiler);

	return nameVarMacroFlagsCompiler;
}

//-----------------------------------------------------------------------

static char *prv_appendDefineLinkerFlags(
							const struct compilador_t *compiler,
							const char *nameSubproject,
							const ArrString *includes_de_terceros,
							ArrString *codeMak)
{
	char *flagsLinker, *nameVarMacroFlagsLinker;

	flagsLinker = compilador_lineCommandLinkerMake(compiler, includes_de_terceros);

	nameVarMacroFlagsLinker = prv_appendDefineVarFlags("FLAGS_LINKER", nameSubproject, flagsLinker, codeMak);

	strbas_destroy(&flagsLinker);

	return nameVarMacroFlagsLinker;
}

//-----------------------------------------------------------------------

static char *prv_createMakSubproject(
							const struct prv_program_t *program,
							const struct compilador_t *compiler,
							const ArrString *ficherosInclude,
							const char *dirMakeProject)
{
	char *nameMak, *fileMake, *varFlagsLinker;
	char *nameSubproject, *nameProgram, *varFlagsCompiler;
	ArrString *codeMak;

	assert_no_null(program);

	codeMak = arrcad_CreaString(0);

	nameSubproject = fichsrc_nombre_base(program->fileMain);
	nameMak = nomfich_cambia_extension(nameSubproject, PRV_EXTENSION_MAK);
	nameProgram = compilador_nombre_ejecutable(compiler, program->fileMain);
	fileMake = nomfich_append_a_directorio(dirMakeProject, nameMak);

	prv_appendHeader(nameSubproject, codeMak);

	prv_appendDefineMacroObjs(program->filesSource, compiler, codeMak);

	varFlagsCompiler = prv_appendDefineCompilerFlags(
							compiler,
							nameSubproject,
							program->includes_de_terceros,
							ficherosInclude,
							codeMak);

	varFlagsLinker = prv_appendDefineLinkerFlags(compiler, nameSubproject, program->includes_de_terceros, codeMak);

	prv_appendCreateExecutable(compiler, varFlagsLinker, nameProgram, codeMak);

	prv_appendCompileSources(program->filesSource, varFlagsCompiler, compiler, codeMak);

	filetxt_escribe_fichero_texto(fileMake, codeMak);

	arrcad_Destruye(&codeMak);
	strbas_destroy(&fileMake);
	strbas_destroy(&varFlagsCompiler);
	strbas_destroy(&varFlagsLinker);
	strbas_destroy(&nameSubproject);
	strbas_destroy(&nameProgram);

	return nameMak;
}

//-----------------------------------------------------------------------

static char *prv_dependsPrograms(const ArrTipo(prv_program_t) *programs, const struct compilador_t *compilador)
{
	char *txt_programs;
	ArrString *nombres_ejecutables;
	unsigned long i, num;

	num = array_numElementos(prv_program_t, programs);
	nombres_ejecutables = arrcad_CreaString(num);

	for (i = 0; i < num; i++)
	{
		const struct prv_program_t *program;
		char *nombre_fichero;

		program = array_Get(prv_program_t, programs, i);
		assert_no_null(program);

		nombre_fichero = compilador_nombre_ejecutable(compilador, program->fileMain);
		arrcad_SetDestruyendoCadena(nombres_ejecutables, i, &nombre_fichero);
	}

	txt_programs = arrcad_texto_separado_por_espacios(nombres_ejecutables);

	arrcad_Destruye(&nombres_ejecutables);

	return txt_programs;
}

//-----------------------------------------------------------------------

static struct genmak_t *prv_create(const char *nameProject, ArrTipo(fichsrc_t) **filesIncludeProject, ArrTipo(prv_program_t) **programs)
{
	struct genmak_t *generator_mak;

	generator_mak = MALLOC(struct genmak_t);

	generator_mak->nameProject = strbas_copyString(nameProject);
	generator_mak->filesIncludeProject = ASSIGN_PP_NO_NULL(filesIncludeProject, ArrTipo(fichsrc_t));
	generator_mak->programs = ASSIGN_PP_NO_NULL(programs, ArrTipo(prv_program_t));

	return generator_mak;
}

//-----------------------------------------------------------------------

struct genmak_t *genmak_createDefault(
							const char *nameProject,
							const ArrTipo(fichsrc_t) *filesIncludesProject_opc)
{
	ArrTipo(prv_program_t) *programs;
	ArrTipo(fichsrc_t) *filesIncludesProjectCopy;

	if (filesIncludesProject_opc != NULL)
		filesIncludesProjectCopy = array_Copia(fichsrc_t, filesIncludesProject_opc, fichsrc_copia);
	else
		filesIncludesProjectCopy = array_CreaPuntero(0, fichsrc_t);

	programs = array_CreaPuntero(0, prv_program_t);

	return prv_create(nameProject, &filesIncludesProjectCopy, &programs);
}

//-----------------------------------------------------------------------

void genmak_destroy(struct genmak_t **generator_mak)
{
	assert_no_null(generator_mak);

	strbas_destroy(&(*generator_mak)->nameProject);
	array_Destruye(prv_program_t, &(*generator_mak)->programs, prv_destroyProgram);
	array_Destruye(fichsrc_t, &(*generator_mak)->filesIncludeProject, fichsrc_destruye);

	FREE_T(generator_mak, struct genmak_t);
}

//-----------------------------------------------------------------------

void genmak_appendProgram(
						struct genmak_t *generatorMak,
						const struct fichsrc_t *fileMain,
						const ArrTipo(fichsrc_t) **filesSource,
						const ArrTipo(fichsrc_t) **filesIncludes,
						ArrString **includes_de_terceros)
{
	struct prv_program_t *program;
	struct fichsrc_t *fileMainCopy;
	ArrTipo(fichsrc_t) *filesSourceCopy, *filesIncludesCopy;

	assert_no_null(generatorMak);
	assert_no_null(filesSource);
	assert_no_null(filesIncludes);

	fileMainCopy = fichsrc_copia(fileMain);
	filesSourceCopy = array_Copia(fichsrc_t, *filesSource, fichsrc_copia);
	filesIncludesCopy = array_Copia(fichsrc_t, *filesIncludes, fichsrc_copia);

	program = prv_createProgram(&fileMainCopy, &filesSourceCopy, &filesIncludesCopy, includes_de_terceros);

	array_Anyadir(prv_program_t, generatorMak->programs, program);

	array_DestruyeConst(fichsrc_t, filesSource);
}

//-----------------------------------------------------------------------

static void prv_appendAllTarget(
					const ArrTipo(prv_program_t) *programs,
					const struct compilador_t *compilador,
					ArrString *codeMak)
{
	char *txt_depends_programs;
	ArrString *strAllTarget;

	prv_appendCommentConst(codeMak, "All Target");
	txt_depends_programs = prv_dependsPrograms(programs, compilador);

	strAllTarget = arrcad_CreaString(0);
	prv_appendEcho("Terminado toda la compilación", strAllTarget);

	prv_appendTargetDepends("all", txt_depends_programs, &strAllTarget, codeMak);
	arrcad_Anyadir(codeMak, "");

	strbas_destroy(&txt_depends_programs);
}

//-----------------------------------------------------------------------

static ArrString *prv_makSubprojects(
							const ArrTipo(prv_program_t) *programs,
							const struct compilador_t *compilador,
							const ArrString *ficherosInclude,
							const char *dirMakeProject)
{
	ArrString *makSubprojects;
	unsigned long i, num;

	num = array_numElementos(prv_program_t, programs);
	makSubprojects = arrcad_CreaString(num);

	for (i = 0; i < num; i++)
	{
		const struct prv_program_t *program;
		char *fileMakProgram;

		program = array_Get(prv_program_t, programs, i);
		fileMakProgram = prv_createMakSubproject(program, compilador, ficherosInclude, dirMakeProject);
		arrcad_SetDestruyendoCadena(makSubprojects, i, &fileMakProgram);
	}

	return makSubprojects;
}

//-----------------------------------------------------------------------

static void prv_definitionVariables(const struct compilador_t *compiler, ArrString *codeMak)
{
	const char *nameCompiler, *nameLinker;

	nameCompiler = compilador_nombre_compilador(compiler);
	nameLinker = compilador_nombre_linker(compiler);

	prv_appendMacroValue(PRV_MACRO_COMPILER, nameCompiler, codeMak);
	prv_appendMacroValue(PRV_MACRO_LINKER, nameLinker, codeMak);

	arrcad_Anyadir(codeMak, "");
}

//-----------------------------------------------------------------------

static void i_copyFiles(
					const ArrTipo(fichsrc_t) *files,
					const char *dirMakeProjectSrc,
					struct marco_t *marco)
{
	unsigned long i, num;

	num = array_numElementos(fichsrc_t, files);

	for (i = 0; i < num; i++)
	{
		const struct fichsrc_t *fichero_src;

		fichero_src = array_Get(fichsrc_t, files, i);
		fichsrc_copia_a_directorio(fichero_src, dirMakeProjectSrc, marco);
	}
}

//-----------------------------------------------------------------------

static void prv_copyFilesProgram(
					const struct prv_program_t *program,
					const char *dirMakeProjectSrc,
					struct marco_t *marco)
{
	i_copyFiles(program->filesSource, dirMakeProjectSrc, marco);
	i_copyFiles(program->filesIncludes, dirMakeProjectSrc, marco);
}

//-----------------------------------------------------------------------

static void i_createDictory(const char *dirMakeProject, const char *dir)
{
	char *dirMakeWithDir;

	dirMakeWithDir = nomfich_append_a_directorio(dirMakeProject, dir);
	nomfich_crear_directorios_si_no_existen(dirMakeWithDir);
	strbas_destroy(&dirMakeWithDir);
}

//-----------------------------------------------------------------------

void genmak_copyFiles(
					const struct genmak_t *generatorMak,
					const char *dirMake,
					struct marco_t *marco)
{
	char *dirMakeProject, *dirMakeProjectSrc;
	unsigned long i, num;

	dirMakeProject = nomfich_append_a_directorio(dirMake, generatorMak->nameProject);
	dirMakeProjectSrc = nomfich_append_a_directorio(dirMakeProject, PRV_ONLY_DIR_SOURCE_MAKE);

	i_createDictory(dirMakeProject, PRV_ONLY_DIR_SOURCE_MAKE);
	i_createDictory(dirMakeProject, PRV_ONLY_DIR_OBJS_MAKE);
	i_createDictory(dirMakeProject, PRV_ONLY_DIR_BIN_MAKE);

	num = array_numElementos(prv_program_t, generatorMak->programs);

	for (i = 0; i < num; i++)
	{
		const struct prv_program_t *program;

		program = array_Get(prv_program_t, generatorMak->programs, i);
		prv_copyFilesProgram(program, dirMakeProjectSrc, marco);
	}

	i_copyFiles(generatorMak->filesIncludeProject, dirMakeProjectSrc, marco);

	strbas_destroy(&dirMakeProject);
	strbas_destroy(&dirMakeProjectSrc);
}

//-----------------------------------------------------------------------

static ArrString *prv_fichInSrc(const ArrTipo(fichsrc_t) *filesSource)
{
	ArrString *fichInSrc;
	unsigned long i, num;

	num = array_numElementos(fichsrc_t, filesSource);
	fichInSrc = arrcad_CreaString(num);

	for (i = 0; i < num; i++)
	{
		const struct fichsrc_t *fileSource;
		char *strFileSource;

		fileSource = array_Get(fichsrc_t, filesSource, i);
		strFileSource = prv_nameFileInSrc(fileSource);
		arrcad_SetDestruyendoCadena(fichInSrc, i, &strFileSource);
	}

	return fichInSrc;
}

//-----------------------------------------------------------------------

static void prv_appendCleanDir(const char *dirCleaned, ArrString *cleanCommands)
{
	char *echoCleaning, *commandClean;

	echoCleaning = strbas_printf("Limpiando...%s", dirCleaned);
	arrcad_AnyadirDestruyendoCadena(cleanCommands, &echoCleaning);
	prv_appendEcho("Limpiando ", cleanCommands);

	commandClean = strbas_printf("rm -f %s/*", dirCleaned);
	arrcad_AnyadirDestruyendoCadena(cleanCommands, &commandClean);
}

//-----------------------------------------------------------------------

static void prv_appendClean(ArrString *codeMak)
{
	ArrString *cleanCommands;

	cleanCommands = arrcad_CreaString(0);
	prv_appendCleanDir(PRV_DIR_OBJS_MAKE, cleanCommands);
	prv_appendCleanDir(PRV_DIR_BIN_MAKE, cleanCommands);

	prv_appendTargetDepends("clean", "", &cleanCommands, codeMak);
	arrcad_Anyadir(codeMak, "");
}

//-----------------------------------------------------------------------

void genmak_generateMak(
					const struct genmak_t *generatorMak,
					const struct compilador_t *compiler,
					const char *dirMake)
{
	ArrString *codeMak, *includesMak, *ficherosInclude;
	char *nameMakefile, *dirMakeProject;

	assert_no_null(generatorMak);

	dirMakeProject = nomfich_append_a_directorio(dirMake, generatorMak->nameProject);
	nameMakefile = nomfich_append_a_directorio(dirMakeProject, PRV_MAKEFILE);
	ficherosInclude = prv_fichInSrc(generatorMak->filesIncludeProject);

	includesMak = prv_makSubprojects(generatorMak->programs, compiler, ficherosInclude, dirMakeProject);

	codeMak = arrcad_CreaString(0);

	prv_appendHeader(generatorMak->nameProject, codeMak);

	prv_definitionVariables(compiler, codeMak);

	prv_appendIncludes(includesMak, codeMak);

	prv_appendAllTarget(generatorMak->programs, compiler, codeMak);
	prv_appendClean(codeMak);

	filetxt_escribe_fichero_texto(nameMakefile, codeMak);

	arrcad_Destruye(&codeMak);
	arrcad_Destruye(&includesMak);
	arrcad_Destruye(&ficherosInclude);

	strbas_destroy(&nameMakefile);
	strbas_destroy(&dirMakeProject);
}
