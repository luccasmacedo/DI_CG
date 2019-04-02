DCC065 - Computação Gráfica
---------------------------

Para começar, abra o arquivo 'linux.workspace' que contem todos os arquivos 
para Linux. Para windows, entre na pasta 'src' e utilize o projeto windows.cbp.
No projeto do windows os arquivos fontes tem que ser incluídos individualmente.

===============================================================================
Instruções gerais
===============================================================================
Linux

Para compilar os arquivos deste projeto no Linux, alguns pacotes devem estar 
instalados. A seguir o comando para instalar tudo o que é necessário no Linux 
Mint/Ubuntu:

sudo apt-get install g++ codeblocks freeglut3-dev libglew-dev lib32z1-dev zlib1g-dev libalut-dev libpng16-dev libjpeg-dev libglm-dev libglfw3 libglfw3-dev libglfw3-doc libsoil-dev libassimp-dev 

Essas instruções foram testadas no Linux Mint 18.3 64 bits.

===============================================================================

Windows

Não é preciso configurar nada no windows pois os arquivos para este sistema 
já estão presentes neste projeto, que foi criado para ser executado na IDE 
Codeblocks que pode ser baixada gratuitamente no link abaixo:
http://downloads.sourceforge.net/codeblocks

===============================================================================
Correção de erros comuns
===============================================================================

Sistema operacional: Ubuntu 14.04 LTs

Erro: Ao compilar códigos com o GLUI no linux aparece a seguinte mensagem
Inconsistency detected by ld.so: dl-version.c: 224: _dl_check_map_versions: 
Assertion `needed != ((void *)0)' failed!

Solução:
Em "Build Options" selecionar a opção "Linux".
A seguir, vá em "Linker settings" e na opção "Other Linker options" acrescente:
-L/usr/lib/nvidia-304
Substitua a pasta "nvidia-304" pela correspondente do seu sistema operacional.

-------------------------------------------------------------------------------
