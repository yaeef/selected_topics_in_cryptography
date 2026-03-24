<?php
$bin_path = "./ecdsa"; 
$output = "";
$current_mode = "";

if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    $mode = $_POST['mode'];
    $cmd = "";

    // Mapeo según el formulario enviado
    if ($mode === 'G') {
        $current_mode = "ECDSA: Generando Llaves";
        $cmd = "{$bin_path} -G -a " . escapeshellarg($_POST['a']) . " -b " . escapeshellarg($_POST['b']) . 
               " -p " . escapeshellarg($_POST['p']) . " -n " . escapeshellarg($_POST['q_order']) . 
               " -x " . escapeshellarg($_POST['gx']) . " -y " . escapeshellarg($_POST['gy']) . " -z " . escapeshellarg($_POST['gz']);
    } 
    elseif ($mode === 'S') {
        $current_mode = "ECDSA: Firmando Mensaje";
        $cmd = "{$bin_path} -S -a " . escapeshellarg($_POST['a']) . " -b " . escapeshellarg($_POST['b']) . 
               " -p " . escapeshellarg($_POST['p']) . " -n " . escapeshellarg($_POST['q_order']) . 
               " -x " . escapeshellarg($_POST['gx']) . " -y " . escapeshellarg($_POST['gy']) . " -z " . escapeshellarg($_POST['gz']) .
               " -k " . escapeshellarg($_POST['d_priv']) . " -M " . escapeshellarg($_POST['msg']);
    } 
    elseif ($mode === 'V') {
        $current_mode = "ECDSA: Verificando Firma";
        $cmd = "{$bin_path} -v -a " . escapeshellarg($_POST['a']) . " -b " . escapeshellarg($_POST['b']) . 
               " -p " . escapeshellarg($_POST['p']) . " -n " . escapeshellarg($_POST['q_order']) . 
               " -x " . escapeshellarg($_POST['gx']) . " -y " . escapeshellarg($_POST['gy']) . " -z " . escapeshellarg($_POST['gz']) .
               " -M " . escapeshellarg($_POST['msg']) . " -r " . escapeshellarg($_POST['r_sig']) . " -s " . escapeshellarg($_POST['s_sig']) . 
               " -U " . escapeshellarg($_POST['bx']) . " -W " . escapeshellarg($_POST['by']) . " -J " . escapeshellarg($_POST['bz']);
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
    <title>Motor Criptográfico: ECDSA</title>
    <link href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.0.0/css/all.min.css" rel="stylesheet">
    <style>
        :root {
            --bg-dark: #0f172a; --card-dark: #1e293b; --border-dark: #334155;
            --accent-blue: #38bdf8; --accent-green: #10b981; --accent-orange: #fb923c;
            --text-light: #f1f5f9; --text-muted: #94a3b8;
            --term-bg: #1e1e1e; --term-text: #d4d4d4;
        }
        body { font-family: 'Segoe UI', sans-serif; background: var(--bg-dark); color: var(--text-light); padding: 20px; }
        .container { max-width: 1300px; margin: auto; background: var(--card-dark); padding: 25px; border-radius: 12px; }
        header { border-bottom: 2px solid var(--border-dark); margin-bottom: 25px; padding-bottom: 10px; display: flex; justify-content: space-between; }
        .grid { display: grid; grid-template-columns: repeat(auto-fit, minmax(380px, 1fr)); gap: 20px; align-items: start; }
        .card { background: #1e293b; border: 1px solid var(--border-dark); border-radius: 10px; padding: 18px; border-top: 4px solid var(--accent-blue); }
        .input-group { display: grid; grid-template-columns: repeat(auto-fit, minmax(70px, 1fr)); gap: 8px; margin-bottom: 12px; }
        label { display: block; font-size: 0.65rem; font-weight: bold; color: var(--text-muted); text-transform: uppercase; margin-bottom: 3px; }
        input { width: 100%; background: var(--bg-dark); border: 1px solid var(--border-dark); border-radius: 4px; padding: 6px; color: white; font-family: monospace; box-sizing: border-box; }
        button { width: 100%; border: none; padding: 12px; border-radius: 6px; font-weight: bold; cursor: pointer; transition: 0.2s; margin-top: 10px; }
        .btn-gen { background: var(--accent-orange); color: white; }
        .btn-sign { background: var(--accent-blue); color: var(--bg-dark); }
        .btn-verify { background: var(--accent-green); color: var(--bg-dark); }
        .point-label { color: var(--accent-blue); font-size: 0.75rem; margin: 10px 0 5px 0; font-weight: bold; text-align: center; background: #0f172a; padding: 4px; border-radius: 4px; }
        .terminal-box { margin-top: 25px; border-radius: 8px; overflow: hidden; border: 1px solid #404040; position: relative; }
        .terminal-header { background: #2d2d2d; padding: 8px 15px; display: flex; justify-content: space-between; font-size: 0.8rem; }
        pre { background: var(--term-bg); color: var(--term-text); padding: 20px; margin: 0; font-family: monospace; min-height: 120px; overflow-x: auto; }
        .loader { display: none; position: absolute; top: 50%; left: 50%; transform: translate(-50%, -50%); border: 3px solid #333; border-top: 3px solid var(--accent-blue); border-radius: 50%; width: 30px; height: 30px; animation: spin 1s linear infinite; }
        @keyframes spin { 0% { transform: rotate(0deg); } 100% { transform: rotate(360deg); } }
    </style>
</head>
<body>

<div class="container">
    <header>
        <h1><i class="fas fa-microchip"></i> Motor Criptográfico: ECDSA</h1>
        <div style="font-size: 0.8rem; color: var(--text-muted);">Yair Alejandro Erreguin Franco | STIC | ESCOM</div>
    </header>

    <div class="grid">
        <div class="card" style="border-top-color: var(--accent-orange);">
            <h3><i class="fas fa-key"></i> KeyGen (-G)</h3>
            <form method="POST">
                <input type="hidden" name="mode" value="G">
                <div class="point-label">Curva y Generador</div>
                <div class="input-group">
                    <div><label>a</label><input type="text" name="a" required></div>
                    <div><label>b</label><input type="text" name="b" required></div>
                    <div><label>p</label><input type="text" name="p" required></div>
                </div>
                <div class="input-group">
                    <div><label>Gx</label><input type="text" name="gx" required></div>
                    <div><label>Gy</label><input type="text" name="gy" required></div>
                    <div><label>Gz</label><input type="text" name="gz" value="1" required></div>
                </div>
                <label>n (Orden)</label>
                <input type="text" name="q_order" required>
                <button type="submit" class="btn-gen">Generar Par de Llaves</button>
            </form>
        </div>

        <div class="card">
            <h3><i class="fas fa-pen-nib"></i> Sign (-S)</h3>
            <form method="POST">
                <input type="hidden" name="mode" value="S">
                <div class="point-label">Parámetros de la Curva</div>
                <div class="input-group">
                    <div><label>a</label><input type="text" name="a" required></div>
                    <div><label>b</label><input type="text" name="b" required></div>
                    <div><label>p</label><input type="text" name="p" required></div>
                </div>
                <div class="input-group">
                    <div><label>Gx</label><input type="text" name="gx" required></div>
                    <div><label>Gy</label><input type="text" name="gy" required></div>
                    <div><label>Gz</label><input type="text" name="gz" value="1" required></div>
                </div>
                <label>n (Orden)</label>
                <input type="text" name="q_order" required style="margin-bottom:10px;">
                
                <div class="point-label">Secretos</div>
                <div class="input-group">
                    <div style="grid-column: span 2;"><label>d (Llave Privada)</label><input type="text" name="d_priv" required></div>
                </div>
                <label>M (Mensaje Numérico)</label>
                <input type="text" name="msg" required>
                <button type="submit" class="btn-sign">Firmar Mensaje</button>
            </form>
        </div>

        <div class="card" style="border-top-color: var(--accent-green);">
            <h3><i class="fas fa-check-double"></i> Verify (-v)</h3>
            <form method="POST">
                <input type="hidden" name="mode" value="V">
                <div class="point-label">Parámetros de la Curva</div>
                <div class="input-group">
                    <div><label>a</label><input type="text" name="a" required></div>
                    <div><label>b</label><input type="text" name="b" required></div>
                    <div><label>p</label><input type="text" name="p" required></div>
                </div>
                <div class="input-group">
                    <div><label>Gx</label><input type="text" name="gx" required></div>
                    <div><label>Gy</label><input type="text" name="gy" required></div>
                    <div><label>Gz</label><input type="text" name="gz" value="1" required></div>
                </div>
                <label>n (Orden)</label>
                <input type="text" name="q_order" required style="margin-bottom:10px;">

                <div class="point-label">Firma y Llave Pública (U, W, J)</div>
                <div class="input-group">
                    <div><label>r</label><input type="text" name="r_sig" required></div>
                    <div><label>s</label><input type="text" name="s_sig" required></div>
                </div>
                <div class="input-group">
                    <div><label>Bx (U)</label><input type="text" name="bx" required></div>
                    <div><label>By (W)</label><input type="text" name="by" required></div>
                    <div><label>Bz (J)</label><input type="text" name="bz" value="1" required></div>
                </div>
                <label>M (Mensaje Numérico)</label>
                <input type="text" name="msg" required>
                <button type="submit" class="btn-verify">Verificar Firma</button>
            </form>
        </div>
    </div>

    <div class="terminal-box">
        <div class="terminal-header">
            <div><i class="fas fa-terminal"></i> Output — <?php echo $current_mode; ?></div>
        </div>
        <div id="loader" class="loader"></div>
        <pre id="output-text"><?php echo $output ? htmlspecialchars($output) : "// Esperando entrada..."; ?></pre>
    </div>
</div>

<script>
    document.querySelectorAll('form').forEach(f => {
        f.addEventListener('submit', () => {
            document.getElementById('output-text').style.opacity = '0.3';
            document.getElementById('loader').style.display = 'block';
        });
    });
</script>

</body>
</html>
