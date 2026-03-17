<?php
$bin_path = "./dsa"; 
$output = "";
$current_mode = "";

if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    $mode = $_POST['mode'];
    $cmd = "";

    switch ($mode) {
        case 'K':
            $current_mode = "DSA: Generación de Parámetros y Llaves";
            $cmd = "{$bin_path} -K";
            break;
            
        case 'S':
            $current_mode = "DSA: Generación de Firma Digital";
            $cmd = "{$bin_path} -S -p " . escapeshellarg($_POST['p']) . 
                   " -q " . escapeshellarg($_POST['q']) . 
                   " -g " . escapeshellarg($_POST['g']) . 
                   " -B " . escapeshellarg($_POST['B']) . 
                   " -d " . escapeshellarg($_POST['d']) . 
                   " -m " . escapeshellarg($_POST['m']); 
            break;

        case 'V':
            $current_mode = "DSA: Verificación de Firma";
            $cmd = "{$bin_path} -V -p " . escapeshellarg($_POST['p']) . 
                   " -q " . escapeshellarg($_POST['q']) . 
                   " -g " . escapeshellarg($_POST['g']) . 
                   " -B " . escapeshellarg($_POST['B']) . 
                   " -r " . escapeshellarg($_POST['r']) . 
                   " -s " . escapeshellarg($_POST['s']) . 
                   " -m " . escapeshellarg($_POST['m']); 
            break;
    }

    if ($cmd) {
        $output = shell_exec($cmd . " 2>&1"); 
    }
}
?>

<!DOCTYPE html>
<html lang="es">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Motor Criptográfico: DSA</title>
    <link href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.0.0/css/all.min.css" rel="stylesheet">
    <style>
        :root {
            --bg-dark: #0f172a;
            --card-dark: #1e293b;
            --border-dark: #334155;
            --accent-blue: #38bdf8;
            --accent-green: #10b981;
            --accent-purple: #a855f7;
            --text-light: #f1f5f9;
            --text-muted: #94a3b8;
            --term-bg: #1e1e1e;
            --term-text: #d4d4d4;
            --term-border: #404040;
        }

        input::-webkit-outer-spin-button, input::-webkit-inner-spin-button { -webkit-appearance: none; margin: 0; }
        input[type=number] { -moz-appearance: textfield; }

        body { font-family: 'Segoe UI', system-ui, sans-serif; background: var(--bg-dark); color: var(--text-light); margin: 0; padding: 20px; }
        .container { max-width: 1000px; margin: auto; background: var(--card-dark); padding: 30px; border-radius: 12px; }
        header { border-bottom: 2px solid var(--border-dark); margin-bottom: 30px; padding-bottom: 15px; display: flex; justify-content: space-between; align-items: center; }
        h1 { color: var(--accent-blue); margin: 0; font-size: 1.5rem; }
        
        .grid { display: grid; grid-template-columns: repeat(auto-fit, minmax(300px, 1fr)); gap: 20px; }
        .card { background: #1e293b; border: 1px solid var(--border-dark); border-radius: 10px; padding: 20px; border-top: 4px solid var(--accent-blue); }
        .card.green { border-top-color: var(--accent-green); }
        .card.purple { border-top-color: var(--accent-purple); }
        
        .card h3 { margin-top: 0; font-size: 1rem; display: flex; align-items: center; gap: 8px; text-transform: uppercase; color: var(--text-light); border-bottom: 1px solid var(--border-dark); padding-bottom: 10px; margin-bottom: 15px;}
        
        .input-group { display: grid; grid-template-columns: repeat(auto-fit, minmax(80px, 1fr)); gap: 10px; margin-bottom: 15px; }
        label { display: block; font-size: 0.75rem; font-weight: bold; color: var(--text-muted); text-transform: uppercase; margin-bottom: 4px; }
        input { width: 100%; background: var(--bg-dark); border: 1px solid var(--border-dark); border-radius: 6px; padding: 8px; color: white; box-sizing: border-box; font-family: monospace;}
        input:focus { border-color: var(--accent-blue); outline: none; }
        
        button { width: 100%; color: var(--bg-dark); border: none; padding: 12px; border-radius: 6px; font-weight: bold; cursor: pointer; transition: 0.2s; margin-top: 10px; }
        button.blue-btn { background: var(--accent-blue); }
        button.green-btn { background: var(--accent-green); }
        button.purple-btn { background: var(--accent-purple); color: white; }
        button:hover { filter: brightness(1.1); }

        .point-label { color: var(--accent-blue); font-size: 0.8rem; margin: 10px 0 5px 0; font-weight: bold; text-align: center; background: #0f172a; padding: 5px; border-radius: 4px; }
        .card.green .point-label { color: var(--accent-green); }
        .card.purple .point-label { color: var(--accent-purple); }

        .terminal-box { margin-top: 30px; border-radius: 8px; overflow: hidden; border: 1px solid var(--term-border); box-shadow: 0 10px 30px rgba(0,0,0,0.5); position: relative; }
        .terminal-header { background: #2d2d2d; padding: 10px 15px; display: flex; justify-content: space-between; align-items: center; border-bottom: 1px solid var(--term-border); }
        .terminal-controls { display: flex; gap: 6px; }
        .dot { width: 12px; height: 12px; border-radius: 50%; }
        .r { background: #ff5f56; } .y { background: #ffbd2e; } .g { background: #27c93f; }
        .terminal-info { font-family: monospace; font-size: 0.8rem; color: #858585; }
        
        pre { background: var(--term-bg); color: var(--term-text); padding: 25px; margin: 0; font-family: 'Consolas', 'Monaco', monospace; font-size: 0.95rem; overflow-x: auto; min-height: 150px; line-height: 1.6; transition: opacity 0.3s; }

        .loader-container { display: none; position: absolute; top: 50%; left: 50%; transform: translate(-50%, -50%); text-align: center; z-index: 10; }
        .loader { border: 4px solid #333; border-top: 4px solid var(--accent-blue); border-radius: 50%; width: 40px; height: 40px; animation: spin 1s linear infinite; margin: 0 auto 10px; }
        @keyframes spin { 0% { transform: rotate(0deg); } 100% { transform: rotate(360deg); } }
        .loading-text { color: var(--accent-blue); font-size: 0.8rem; font-family: monospace; }
    </style>
</head>
<body>

<div class="container">
    <header>
        <h1><i class="fas fa-signature"></i> Firma Digital DSA</h1>
        <div style="font-size: 0.8rem; color: var(--text-muted);">Yair Alejandro Erreguin Franco | STIC | ESCOM</div>
    </header>

    <div class="grid">
        <div class="card purple">
            <h3><i class="fas fa-dice"></i> Fase 1: Key Generation</h3>
            <p style="font-size: 0.8rem; color: var(--text-muted); margin-top: -5px;">Genera (p, q, g) y el par (B, d)</p>
            <form method="POST">
                <input type="hidden" name="mode" value="K">
                <div class="point-label">Parámetros Automáticos</div>
                <button type="submit" class="purple-btn">Generar Llaves</button>
            </form>
        </div>

        <div class="card">
            <h3><i class="fas fa-file-signature"></i> Fase 2: Firmar</h3>
            <p style="font-size: 0.8rem; color: var(--text-muted); margin-top: -5px;">Genera (r, s) para un mensaje m</p>
            <form method="POST">
                <input type="hidden" name="mode" value="S">
                
                <div class="point-label">Dominio (p, q, g)</div>
                <div class="input-group">
                    <div><label>p</label><input type="text" name="p" required></div>
                    <div><label>q</label><input type="text" name="q" required></div>
                    <div><label>g</label><input type="text" name="g" required></div>
                </div>

                <div class="point-label">Llaves y Datos</div>
                <div class="input-group">
                    <div><label>B (Púb)</label><input type="text" name="B" required title="Llave pública"></div>
                    <div><label>d (Priv)</label><input type="text" name="d" required title="Llave privada"></div>
                </div>
                <div><label>Mensaje (m)</label><input type="text" name="m" required></div>

                <button type="submit" class="blue-btn">Firmar Mensaje</button>
            </form>
        </div>

        <div class="card green">
            <h3><i class="fas fa-certificate"></i> Fase 3: Verificar</h3>
            <p style="font-size: 0.8rem; color: var(--text-muted); margin-top: -5px;">Valida r, s con B y m</p>
            <form method="POST">
                <input type="hidden" name="mode" value="V">
                
                <div class="input-group">
                    <div><label>p</label><input type="text" name="p" required></div>
                    <div><label>q</label><input type="text" name="q" required></div>
                    <div><label>g</label><input type="text" name="g" required></div>
                </div>

                <div class="point-label">Firma (r, s)</div>
                <div class="input-group">
                    <div><label>r</label><input type="text" name="r" required></div>
                    <div><label>s</label><input type="text" name="s" required></div>
                </div>

                <div class="point-label">Validación</div>
                <div class="input-group">
                    <div style="grid-column: span 2;"><label>B (Llave Pública)</label><input type="text" name="B" required></div>
                </div>
                <div><label>Mensaje (m)</label><input type="text" name="m" required></div>

                <button type="submit" class="green-btn">Verificar Firma</button>
            </form>
        </div>
    </div>

    <div class="terminal-box">
        <div class="terminal-header">
            <div class="terminal-controls"><div class="dot r"></div><div class="dot y"></div><div class="dot g"></div></div>
            <div class="terminal-info">GMP CLI Output — <?php echo $current_mode; ?></div>
        </div>
        
        <div id="loader" class="loader-container">
            <div class="loader"></div>
            <div class="loading-text">Ejecutando DSA en C...</div>
        </div>

        <pre id="output-text"><?php echo $output ? htmlspecialchars($output) : "// Sistema inactivo. Esperando parámetros."; ?></pre>
    </div>
</div>

<script>
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
