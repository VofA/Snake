#!/bin/bash

mkdir -p build
cd build || exit

rm -rf ./*
cmake ../sources/

while true
do
	clear
	echo -n "-- Применение стилей: "
	find ../sources/ -name '*.hpp' -or -name '*.cpp' | xargs clang-format -i -style=file $1
	echo "Готово"

	echo -n "-- Копирование файлов: "
	# cp ../sources/FILE FILE
	echo "Готово"

	echo "-- Компиляция: Запуск"
	make
	echo "-- Компиляция: Завершение"

	read -r operation

	if [ "$operation" ] && [ "$operation" = "cmake" ]
	then
		clear
		rm -rf ./*
		cmake ../sources/
	fi
done
