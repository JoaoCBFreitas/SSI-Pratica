build: passthrough.c permissoes.c
	@echo "A compilar"
	gcc -Wall passthrough.c permissoes.c `pkg-config fuse3 --cflags --libs` -o passthrough
	make mount
	@echo "Fim da compilação"

mount:
	@echo "Criando Diretoria"
	@echo "Conteudo do ficheiro" > ~/teste.txt
	sudo mkdir /mnt/SSI/
	sudo chmod 777 /mnt/SSI/
	@echo "Diretoria criada"
exec:
	@echo "Executando"
	./passthrough -f /mnt/SSI/

clean:
	rm -f passthrough
	if mount | grep /mnt/SSI > /dev/null; then sudo umount /mnt/SSI/; fi
	sudo rm -r /mnt/SSI/
	sudo rm ~/teste.txt

