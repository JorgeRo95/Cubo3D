#include "funcoes.h"

GLuint loadBMP_custom(const char * imagepath) {
	unsigned char header[54]; // Cada arquivo BMP come�a com um cabe�alho 54-bytes 
	// dataPos indica onde os dados come�am no arquivo
	unsigned int dataPos, width, height, imageSize;
	unsigned char * data; // dados RGB

	FILE * file = fopen(imagepath, "rb");
	if (!file) {
		printf("Imagem nao pode ser aberta\n");
		return 0;
	}

	// verifica se o cabe�alho possui 54 bytes
	if (fread(header, 1, 54, file) != 54) {
		printf("N�o e um arquivo BMP correto\n");
		fclose(file);
		return false;
	}

	if (header[0] != 'B' || header[1] != 'M') {
		printf("N�o e um arquivo BMP correto\n");
		fclose(file);
		return 0;
	}

	if (*(int*)&(header[0x1E]) != 0) { printf("Not a correct BMP file\n");    fclose(file); return 0; }
	if (*(int*)&(header[0x1C]) != 24) { printf("Not a correct BMP file\n");    fclose(file); return 0; }


	//Lendo inteiros do array de bytes
	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	width = *(int*)&(header[0x12]);
	height = *(int*)&(header[0x16]);

	// Alguns arquivos BMP est�o mal formatados, pode ter falta de informa��o
	if (imageSize == 0) imageSize = width * height * 3; // 3 : um byte para cada componente Red, Green e Blue.
	if (dataPos == 0) dataPos = 54; // O cabe�alho BMP � feito desta maneira

	//Cria um buffer
	data = new unsigned char[imageSize];

	//L� os dados atuais do arquivo para o buffer
	fread(data, 1, imageSize, file);

	// Tudo na mem�ria agora o arquivo pode ser fechado
	fclose(file);

	//Criar uma textura OpenGL 
	GLuint textureID;
	glGenTextures(1, &textureID);

	// "Liga" a nova textura criada: todas as futuras fun��es de textura ir�o modificar essa textura
	glBindTexture(GL_TEXTURE_2D, textureID);

	//Damos a imagem para o OpenGL 
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

	delete[] data; // Liberando a mem�ria

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//Quando estiver aumentando a imagem (N�o o maior mipmap disponivel), usar o filtro linear
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//Quando estiver reduzindo a imagem, usar uma mistura linear para dois mimaps, cada um filtrado linearmente tamb�m
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//Gerar mipmaps, a proposito (esse coment�rio do autor do tutorial foi o sarcasmo ao qual ele n�o conseguiu resistir)
	glGenerateMipmap(GL_TEXTURE_2D);

	return textureID;
}