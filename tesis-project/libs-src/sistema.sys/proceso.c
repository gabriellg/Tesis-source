// Gestiona el sistema de procesos.

#include "proceso.h"

#include "stdio.h"
#include "unistd.h"
#include "sys/wait.h"
#include "asrtbas.h"
#include "strbas.h"

//----------------------------------------------------------------

static char *i_cambia_barra_directorio_de_windows_a_linux(const char *comando)
{
    size_t i, longitud;
    char *comando_con_barra_directorio_linux;

    comando_con_barra_directorio_linux = strbas_copyString(comando);
    longitud = strbas_length(comando_con_barra_directorio_linux);

    for (i = 0; i < longitud; i++)
    {
        if (comando_con_barra_directorio_linux[i] == '\\')
            comando_con_barra_directorio_linux[i] = '/';
    }

    return comando_con_barra_directorio_linux;
}

//----------------------------------------------------------------

void proceso_lanzar_proceso(const char *comando)
{
	FILE *pipe;
    char *comando_linux;

    comando_linux = i_cambia_barra_directorio_de_windows_a_linux(comando);

	pipe = popen(comando_linux, "r");
	assert_no_null(pipe);
	pclose(pipe);

    strbas_destroy(&comando_linux);
}

//----------------------------------------------------------------

static void i_ejecutaProgramaTest(const char *comando)
{
    char *comando_linux;
	char *args[2] = { NULL, NULL};
	char *envp[2] = { NULL, NULL};

	assert_no_null(comando);

	envp[0] = strbas_copyString("TEST_ON=1");
	envp[1] = NULL;

    comando_linux = i_cambia_barra_directorio_de_windows_a_linux(comando);

	args[0] = comando_linux;
	args[1] = NULL;

	execve(comando, args, envp);

    strbas_destroy(&comando_linux);
    strbas_destroy(&envp[0]);
}

//----------------------------------------------------------------

int proceso_ejecuta_programa_para_test(const char *comando)
{
	int retornoHijo;
	pid_t idHijo;

	idHijo = fork();

	if (idHijo == 0)
		i_ejecutaProgramaTest(comando);
	else
	{
		waitpid(idHijo, &retornoHijo, 0);
	}

	return retornoHijo;
}
