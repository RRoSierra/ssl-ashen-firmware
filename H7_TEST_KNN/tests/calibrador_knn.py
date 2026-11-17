import serial
import time

# Configura tu puerto COM (ej. COM3 en Windows o /dev/ttyUSB0 en Linux)
PORT = '/dev/ttyUSB0' 
BAUD = 115200

try:
    ser = serial.Serial(PORT, BAUD, timeout=1)
    print(f"Conectado al robot en {PORT}")
except Exception as e:
    print(f"Error: {e}")
    exit()

# Limpiar el buffer de lecturas viejas
ser.reset_input_buffer()

print("\n--- HERRAMIENTA DE CALIBRACIÓN K-NN ---")
print("Coloca la pelota en la grilla y escribe la coordenada.")
print("Formato: X Y (ejemplo: 10 25)")
print("Escribe 'q' para guardar y salir.\n")

lut_data = []

while True:
    comando = input("Coordenada X Y > ")
    
    if comando.lower() == 'q':
        break
        
    try:
        x_str, y_str = comando.split()
        x, y = float(x_str), float(y_str)
        
        # Limpiar el buffer para no leer datos viejos
        ser.reset_input_buffer()
        
        # NUEVO: Leer y descartar la primera línea (que probablemente quedó cortada a la mitad)
        ser.readline() 
        
        # Ahora sí, leer una trama completamente fresca y entera
        linea = ser.readline().decode('utf-8', errors='ignore').strip()
        
        datos_crudos = linea.split(',')
        sensores = datos_crudos[2:] 
        
        if len(sensores) == 6:
            # Formatear como una fila de matriz en C
            fila_c = f"    {{{x:.1f}, {y:.1f}, {sensores[0]}, {sensores[1]}, {sensores[2]}, {sensores[3]}, {sensores[4]}, {sensores[5]}}},"
            lut_data.append(fila_c)
            print(f"✅ Guardado: {fila_c}")
        else:
            print(f"⚠️ Error: Trama incompleta. Dato crudo recibido: '{linea}'")
            
    except ValueError:
        print("⚠️ Formato incorrecto. Usa dos números separados por espacio (ej: 0 10).")
    except Exception as e:
        print(f"⚠️ Error de lectura: {e}")
            
    except ValueError:
        print("⚠️ Formato incorrecto. Usa dos números separados por espacio (ej: 0 10).")
    except Exception as e:
        print(f"⚠️ Error de lectura: {e}")

# Al salir, guardar todo en un archivo de texto
if lut_data:
    with open("lut_generada.txt", "w") as f:
        f.write("const float LUT[PUNTOS][8] = {\n")
        f.write("\n".join(lut_data))
        f.write("\n};\n")
    print(f"\n🎉 Calibración finalizada. {len(lut_data)} puntos guardados en 'lut_generada.txt'.")
else:
    print("\nNo se guardaron datos.")

ser.close()