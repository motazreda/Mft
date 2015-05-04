void PrintToFile(char *instruction_ascii){
	FILE *WrFile;
	WrFile = fopen("as_output.txt", "a");
	if(WrFile ==NULL){
		perror("cannot open file for write\n");
		exit(1);
	}

	fprintf(WrFile, instruction_ascii);
	fprintf(WrFile, "\n");
	fclose(WrFile);
}


void CheckFileExists(){
	if (fopen("as_output.txt", "r") != NULL){
		printf("Removing Old File First\n");
		remove("as_output.txt");
	}
}