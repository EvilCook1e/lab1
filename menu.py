import os
import platform
import subprocess

def compile_c(program_name):
    """Компилирует C файл, если нужно"""
    ext = ".exe" if platform.system() == "Windows" else ""
    source = f"{program_name}.c"
    executable = f"{program_name}{ext}"
    
    if os.path.exists(executable):
        return True
    
    if not os.path.exists(source):
        print(f"Файл {source} не найден")
        return False
    
    print(f"Компилирую {source}...")
    cmd = ["gcc", source, "-o", executable]
    result = subprocess.run(cmd, capture_output=True, text=True)
    
    if result.returncode == 0:
        print(f"{program_name} скомпилирован")
        return True
    else:
        print(f"Ошибка компиляции:\n{result.stderr}")
        return False

def run_program(program_name):
    """Запускает скомпилированную программу"""
    ext = ".exe" if platform.system() == "Windows" else ""
    executable = f"{program_name}{ext}"
    
    if not os.path.exists(executable):
        if not compile_c(program_name):
            return
    
    print(f"\n--- Запуск {program_name} ---")
    
    os.system(executable)
    
    print(f"--- Завершение {program_name} ---\n")

def show_menu():
    print("\n" + "="*40)
    print("   ЛАБОРАТОРНАЯ РАБОТА")
    print("   Выберите программу:")
    print("="*40)
    print("1. Программа Попова - Сложение римских чисел (C)")
    print("2. Программа Столкова - (C)")
    print("0. Выход")
    print("="*40)

if __name__ == "__main__":
    while True:
        show_menu()
        choice = input("Ваш выбор: ")
        
        if choice == "1":
            run_program("popov")
        elif choice == "2":
            run_program("stulkov")
        elif choice == "0":
            print("До свидания!")
            break
        else:
            print("Неверный ввод")