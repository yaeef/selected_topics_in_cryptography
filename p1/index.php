<?php
$bin_path = "./practica"; 
$output = "";
$current_mode = "";

if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    $mode = $_POST['mode'];
    $current_mode = "Ejecutando Modo " . $mode;
    $cmd = "";

    // Mapeo exacto de banderas según tu lógica de orquestador
    switch ($mode) {
        case '1': $cmd = "{$bin_path} -1 -p " . escapeshellarg($_POST['p']); break;
        case '2': $cmd = "{$bin_path} -2 -n " . escapeshellarg($_POST['n']); break;
        case '3': $cmd = "{$bin_path} -3 -a " . escapeshellarg($_POST['a']) . " -b " . escapeshellarg($_POST['b']) . " -p " . escapeshellarg($_POST['p']); break;
        case '4': $cmd = "{$bin_path} -4 -a " . escapeshellarg($_POST['a']) . " -b " . escapeshellarg($_POST['b']) . " -p " . escapeshellarg($_POST['p']) . " -x " . escapeshellarg($_POST['x1']) . " -y " . escapeshellarg($_POST['y1']) ." -z" . escapeshellarg($_POST['z1']) . " -X " . escapeshellarg($_POST['x2']) . " -Y " . escapeshellarg($_POST['y2']) . " -Z" . escapeshellarg($_POST['z2']); break;
        case '5': $cmd = "{$bin_path} -5 -a " . escapeshellarg($_POST['a']) . " -b " . escapeshellarg($_POST['b']) . " -p " . escapeshellarg($_POST['p']) . " -x " . escapeshellarg($_POST['x1']) . " -y " . escapeshellarg($_POST['y1']); break;
    }

    if ($cmd) {
        // Redirección de stderr a stdout para capturar errores de GMP
        $output = shell_exec($cmd . " 2>&1"); 
    }
}
?>

<!DOCTYPE html>
<html lang="es">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Práctica 1</title>
    <link href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.0.0/css/all.min.css" rel="stylesheet">
    <style>
        :root {
            --bg-dark: #0f172a;
            --card-dark: #1e293b;
            --border-dark: #334155;
            --accent-blue: #38bdf8;
            --accent-green: #10b981;
            --accent-orange: #f59e0b;
            --text-light: #f1f5f9;
            --text-muted: #94a3b8;
            /* Terminal Theme */
            --term-bg: #1e1e1e;
            --term-text: #d4d4d4;
            --term-blue: #569cd6;
            --term-border: #404040;
        }

        /* Quitar flechas de inputs numéricos */
        input::-webkit-outer-spin-button,
        input::-webkit-inner-spin-button { -webkit-appearance: none; margin: 0; }
        input[type=number] { -moz-appearance: textfield; }

        body { font-family: 'Segoe UI', system-ui, sans-serif; background: var(--bg-dark); color: var(--text-light); margin: 0; padding: 20px; }
        .container { max-width: 1100px; margin: auto; background: var(--card-dark); padding: 30px; border-radius: 12px; }
        header { border-bottom: 2px solid var(--border-dark); margin-bottom: 30px; padding-bottom: 15px; display: flex; justify-content: space-between; align-items: center; }
        h1 { color: var(--accent-blue); margin: 0; font-size: 1.5rem; }
        
        .grid { display: grid; grid-template-columns: repeat(auto-fit, minmax(320px, 1fr)); gap: 20px; }
        .card { background: #1e293b; border: 1px solid var(--border-dark); border-radius: 10px; padding: 20px; border-top: 4px solid var(--accent-blue); display: flex; flex-direction: column; }
        .card h3 { margin-top: 0; font-size: 0.9rem; display: flex; align-items: center; gap: 8px; text-transform: uppercase; color: var(--text-light); }
        
        .input-group { display: grid; grid-template-columns: repeat(auto-fit, minmax(60px, 1fr)); gap: 10px; margin-bottom: 15px; }
        label { display: block; font-size: 0.7rem; font-weight: bold; color: var(--text-muted); text-transform: uppercase; margin-bottom: 4px; }
        input { width: 100%; background: var(--bg-dark); border: 1px solid var(--border-dark); border-radius: 6px; padding: 8px; color: white; box-sizing: border-box; }
        input:focus { border-color: var(--accent-blue); outline: none; }
        
        button { width: 100%; background: var(--accent-blue); color: var(--bg-dark); border: none; padding: 12px; border-radius: 6px; font-weight: bold; cursor: pointer; transition: 0.2s; margin-top: auto; }
        button:hover { filter: brightness(1.1); }

        /* Terminal Estilo VS Code */
        .terminal-box { margin-top: 30px; border-radius: 8px; overflow: hidden; border: 1px solid var(--term-border); box-shadow: 0 10px 30px rgba(0,0,0,0.5); position: relative; }
        .terminal-header { background: #2d2d2d; padding: 10px 15px; display: flex; justify-content: space-between; align-items: center; border-bottom: 1px solid var(--term-border); }
        .terminal-controls { display: flex; gap: 6px; }
        .dot { width: 12px; height: 12px; border-radius: 50%; }
        .r { background: #ff5f56; } .y { background: #ffbd2e; } .g { background: #27c93f; }
        .terminal-info { font-family: monospace; font-size: 0.8rem; color: #858585; }
        
        pre { 
            background: var(--term-bg); 
            color: var(--term-text); 
            padding: 25px; 
            margin: 0; 
            font-family: 'Consolas', 'Monaco', monospace; 
            font-size: 0.95rem; 
            overflow-x: auto; 
            min-height: 150px; 
            line-height: 1.6;
            transition: opacity 0.3s;
        }

        /* Spinner de Carga */
        .loader-container {
            display: none;
            position: absolute;
            top: 50%; left: 50%;
            transform: translate(-50%, -50%);
            text-align: center;
            z-index: 10;
        }
        .loader {
            border: 4px solid #333;
            border-top: 4px solid var(--accent-blue);
            border-radius: 50%;
            width: 40px; height: 40px;
            animation: spin 1s linear infinite;
            margin: 0 auto 10px;
        }
        @keyframes spin { 0% { transform: rotate(0deg); } 100% { transform: rotate(360deg); } }
        .loading-text { color: var(--accent-blue); font-size: 0.8rem; font-family: monospace; }

        .point-label { color: var(--accent-blue); font-size: 0.75rem; margin: 10px 0 5px 0; font-weight: bold; }
    </style>
</head>
<body>

<div class="container">
    <header>
        <h1><i class="fas fa-terminal"></i>Práctica Curvas Elípticas</h1>
        <div style="font-size: 0.8rem; color: var(--text-muted);">Yair Alejandro Erreguin Franco | Selected Topics In Cryptography</div>
    </header>

    <div class="grid">
        <div class="card">
            <h3><i class="fas fa-search"></i> Modo 1: EC's no singulares</h3>
            <form method="POST">
                <input type="hidden" name="mode" value="1">
                <div class="input-group"><div><label>Primo p</label><input type="number" name="p" value="13" required></div></div>
                <button type="submit">Listar Curvas</button>
            </form>
        </div>

        <div class="card" style="border-top-color: var(--accent-green);">
            <h3><i class="fas fa-key"></i> Modo 2: Generar Curva</h3>
            <form method="POST">
                <input type="hidden" name="mode" value="2">
                <div class="input-group"><div><label>Bits (n)</label><input type="number" name="n" value="16" min="4" required></div></div>
                <button type="submit" style="background: var(--accent-green); color: #000;">Generar Curva</button>
            </form>
        </div>

        <div class="card" style="border-top-color: var(--accent-orange);">
            <h3><i class="fas fa-list-ol"></i> Modo 3: Conteo de puntos en EC</h3>
            <form method="POST">
                <input type="hidden" name="mode" value="3">
                <div class="input-group">
                    <div><label>a</label><input type="number" name="a" required></div>
                    <div><label>b</label><input type="number" name="b" required></div>
                    <div><label>p</label><input type="number" name="p" required></div>
                </div>
                <button type="submit" style="background: var(--accent-orange); color: #000;">Calcular Orden</button>
            </form>
        </div>

        <div class="card" style="border-top-color: #8b5cf6;">
            <h3><i class="fas fa-plus"></i> Modo 4: Suma P + Q</h3>
            <form method="POST">
                <input type="hidden" name="mode" value="4">
                <div class="input-group">
                    <div><label>a</label><input type="number" name="a" required></div>
                    <div><label>b</label><input type="number" name="b" required></div>
                    <div><label>p</label><input type="number" name="p" required></div>
                </div>
                <div class="point-label">Puntos P y Q</div>
                <div class="input-group">
                    <input type="number" name="x1" placeholder="x1" required> <input type="number" name="y1" placeholder="y1" required> <input type="number" name="z1" placeholder="z1" required>
                </div>
                <div class="input-group">
                    <input type="number" name="x2" placeholder="x2" required> <input type="number" name="y2" placeholder="y2" required> <input type="number" name="z2" placeholder="z2" required>
                </div>
                <button type="submit" style="background: #8b5cf6; color: white;">Sumar Puntos</button>
            </form>
        </div>

        <div class="card" style="border-top-color: #ec4899;">
            <h3><i class="fas fa-redo"></i> Modo 5: Duplicar (2P)</h3>
            <form method="POST">
                <input type="hidden" name="mode" value="5">
                <div class="input-group">
                    <div><label>a</label><input type="number" name="a" required></div>
                    <div><label>b</label><input type="number" name="b" required></div>
                    <div><label>p</label><input type="number" name="p" required></div>
                </div>
                <div class="point-label">Punto P (x, y)</div>
                <div class="input-group">
                    <input type="number" name="x1" placeholder="x" required> <input type="number" name="y1" placeholder="y" required>
                </div>
                <button type="submit" style="background: #ec4899; color: white;">Duplicar Punto</button>
            </form>
        </div>
    </div>

    <div class="terminal-box">
        <div class="terminal-header">
            <div class="terminal-controls"><div class="dot r"></div><div class="dot y"></div><div class="dot g"></div></div>
            <div class="terminal-info">GMP Output — <?php echo $current_mode; ?></div>
        </div>
        
        <div id="loader" class="loader-container">
            <div class="loader"></div>
            <div class="loading-text">Ejecutando :p ...</div>
        </div>

        <pre id="output-text"><?php echo $output ? htmlspecialchars($output) : "// Sistema listo."; ?></pre>
    </div>
</div>

<script>
    // Lógica para mostrar cargador al enviar formularios
    document.querySelectorAll('form').forEach(form => {
        form.addEventListener('submit', function() {
            document.getElementById('output-text').style.opacity = '0.2';
            document.getElementById('loader').style.display = 'block';
            document.querySelectorAll('button').forEach(btn => {
                btn.style.pointerEvents = 'none';
                btn.style.opacity = '0.6';
            });
        });
    });
</script>

</body>
</html>
