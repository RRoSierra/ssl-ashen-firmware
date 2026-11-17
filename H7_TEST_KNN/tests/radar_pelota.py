import serial
import matplotlib.pyplot as plt
import matplotlib.animation as animation

# --- CONFIGURACIÓN ---
PORT = '/dev/ttyUSB0'  # Cambia a COM3, COM4 etc., si usas Windows
BAUD = 115200

# Conectar al STM32
try:
    ser = serial.Serial(PORT, BAUD, timeout=0.1)
    print(f"Conectado a {PORT}")
except Exception as e:
    print(f"Error abriendo puerto: {e}")
    exit()

# --- CONFIGURACIÓN DE LA GRÁFICA 2D ---
fig, ax = plt.subplots(figsize=(6, 8))
# Limites del plano X (Ancho del dribbler, ej. +/- 60mm) e Y (Profundidad, ej. -10 a 80mm)
ax.set_xlim(-60, 60)
ax.set_ylim(-10, 80)
ax.set_title("Radar k-NN: Captura de Balón", fontsize=14, fontweight='bold')
ax.set_xlabel("Eje X Lateral (mm)")
ax.set_ylabel("Eje Y Frontal (mm)")
ax.grid(True, linestyle='--', alpha=0.7)

# Dibujar el mecanismo del Dribbler (Línea negra gruesa en Y=0)
ax.axhline(0, color='black', linewidth=6, label='Rodillo Dribbler')

# Inicializar el marcador gráfico de la pelota de golf
ball, = ax.plot([], [], 'ro', markersize=25, markeredgecolor='darkred', label='Balón')
ax.legend(loc="upper right")

# Texto para mostrar la telemetría cruda en la pantalla
telemetry_text = ax.text(-55, 75, "", fontsize=10, color='blue')

# --- FUNCIÓN DE ACTUALIZACIÓN (Bucle Principal) ---
# --- FUNCIÓN DE ACTUALIZACIÓN (Bucle Principal) ---
def update(frame):
    try:
        # Leer todas las líneas disponibles en el buffer serial
        while ser.in_waiting > 0:
            line_bytes = ser.readline()
            line = line_bytes.decode('utf-8', errors='ignore').strip()
            
            # Evitar procesar líneas vacías
            if not line:
                continue

            data = line.split(',')
            
            # Verificar que llegó la trama completa (X, Y + 6 sensores)
            if len(data) == 8:
                x, y = float(data[0]), float(data[1])
                sensores = data[2:] # Los 6 valores crudos
                
                # Actualizar la posición del círculo rojo en el plano
                ball.set_data([x], [y])
                
                # Actualizar el texto en pantalla
                telemetry_text.set_text(f"Pos: ({x:.1f}, {y:.1f}) mm\nSensores: {' | '.join(sensores)}")
            else:
                print(f"Trama incompleta recibida: {line}")

    except ValueError as ve:
        print(f"Error de conversión matemática: {ve} | Trama: '{line}'")
    except Exception as e:
        print(f"Error general: {e}")
        
    return ball, telemetry_text

# Añadimos cache_frame_data=False para eliminar el UserWarning
ani = animation.FuncAnimation(fig, update, interval=20, blit=True, cache_frame_data=False)
plt.tight_layout()
plt.show()

ser.close()