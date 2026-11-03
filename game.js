(() => {
  const canvas = document.getElementById("game");
  const ctx = canvas.getContext("2d");
  const overlay = document.getElementById("overlay");
  const overlayTitle = document.getElementById("overlay-title");
  const overlayMsg = document.getElementById("overlay-msg");
  const startBtn = document.getElementById("start-btn");
  const scoreEl = document.getElementById("score");
  const bestEl = document.getElementById("best");

  const BEST_KEY = "dino-best-score";
  let best = Number(localStorage.getItem(BEST_KEY) || 0);
  bestEl.textContent = String(best);

  const GROUND_Y = 260;
  const GRAVITY = 0.55;
  const JUMP_VELOCITY = -13.5;

  const dinoArt = [
    "            ___",
    "           / ._)",
    "    .-^^^-/ /",
    " __/       /",
    "<__.|_|-|_|",
  ];

  const cactusArt = [
    " _  ",
    "|_| ",
    "|| |",
    "||_|",
  ];

  let state = "ready"; // ready | playing | over
  let score = 0;
  let frame = 0;
  let speed = 6;
  let dino = createDino();
  let cacti = [];
  let nextSpawn = 90;
  let raf = null;

  function createDino() {
    return {
      x: 60,
      y: GROUND_Y,
      vy: 0,
      onGround: true,
      width: 70,
      height: 48,
    };
  }

  function reset() {
    score = 0;
    frame = 0;
    speed = 6;
    dino = createDino();
    cacti = [];
    nextSpawn = 70;
    scoreEl.textContent = "0";
  }

  function spawnCactus() {
    const lastX = cacti.length ? cacti[cacti.length - 1].x : canvas.width;
    const gap = 220 + Math.random() * 220;
    cacti.push({
      x: Math.max(canvas.width + 20, lastX + gap),
      y: GROUND_Y,
      width: 28,
      height: 42,
      scored: false,
    });
  }

  function jump() {
    if (state === "ready") {
      startGame();
      return;
    }
    if (state !== "playing") return;
    if (dino.onGround) {
      dino.vy = JUMP_VELOCITY;
      dino.onGround = false;
    }
  }

  function startGame() {
    reset();
    state = "playing";
    overlay.classList.add("hidden");
    spawnCactus();
    spawnCactus();
    if (!raf) loop();
  }

  function gameOver() {
    state = "over";
    if (score > best) {
      best = score;
      localStorage.setItem(BEST_KEY, String(best));
      bestEl.textContent = String(best);
    }
    overlayTitle.textContent = "Game Over!";
    overlayMsg.textContent = `Final score: ${score}. Press Start or Space to try again.`;
    startBtn.textContent = "Play Again";
    overlay.classList.remove("hidden");
  }

  function update() {
    if (state !== "playing") return;

    frame++;
    speed = 6 + Math.min(8, Math.floor(score / 8));

    dino.vy += GRAVITY;
    dino.y += dino.vy;
    if (dino.y >= GROUND_Y) {
      dino.y = GROUND_Y;
      dino.vy = 0;
      dino.onGround = true;
    }

    nextSpawn--;
    if (nextSpawn <= 0) {
      spawnCactus();
      nextSpawn = 50 + Math.floor(Math.random() * 70);
    }

    for (const c of cacti) {
      c.x -= speed;

      if (!c.scored && c.x + c.width < dino.x) {
        c.scored = true;
        score++;
        scoreEl.textContent = String(score);
      }

      // Collision (body boxes, forgiving)
      const dLeft = dino.x + 22;
      const dRight = dino.x + dino.width - 14;
      const dTop = dino.y - dino.height + 12;
      const dBottom = dino.y - 4;

      const cLeft = c.x + 6;
      const cRight = c.x + c.width - 4;
      const cTop = c.y - c.height + 4;
      const cBottom = c.y;

      const hit =
        dRight > cLeft &&
        dLeft < cRight &&
        dBottom > cTop &&
        dTop < cBottom;

      if (hit) {
        gameOver();
        return;
      }
    }

    cacti = cacti.filter((c) => c.x > -60);
  }

  function drawAscii(lines, x, baselineY, color, fontSize = 14) {
    ctx.fillStyle = color;
    ctx.font = `${fontSize}px "Courier New", monospace`;
    ctx.textBaseline = "bottom";
    for (let i = 0; i < lines.length; i++) {
      const lineY = baselineY - (lines.length - 1 - i) * (fontSize - 1);
      ctx.fillText(lines[i], x, lineY);
    }
  }

  function draw() {
    ctx.clearRect(0, 0, canvas.width, canvas.height);

    // Soft hills
    ctx.fillStyle = "rgba(120, 160, 120, 0.25)";
    ctx.beginPath();
    ctx.ellipse(180, GROUND_Y + 10, 140, 36, 0, 0, Math.PI * 2);
    ctx.ellipse(520, GROUND_Y + 16, 180, 44, 0, 0, Math.PI * 2);
    ctx.ellipse(780, GROUND_Y + 8, 120, 30, 0, 0, Math.PI * 2);
    ctx.fill();

    // Ground
    ctx.strokeStyle = "#3d3429";
    ctx.lineWidth = 2;
    ctx.beginPath();
    ctx.moveTo(0, GROUND_Y + 2);
    ctx.lineTo(canvas.width, GROUND_Y + 2);
    ctx.stroke();

    // Moving dashes on ground
    ctx.fillStyle = "#8a734f";
    const offset = (frame * speed) % 40;
    for (let x = -offset; x < canvas.width; x += 40) {
      ctx.fillRect(x, GROUND_Y + 8, 18, 3);
    }

    drawAscii(dinoArt, dino.x, dino.y, "#2a3d2e", 13);

    for (const c of cacti) {
      drawAscii(cactusArt, c.x, c.y, "#1f5c3a", 14);
    }

    if (state === "playing") {
      ctx.fillStyle = "#3d3429";
      ctx.font = 'bold 16px "Segoe UI", sans-serif';
      ctx.fillText(`Score ${score}`, canvas.width - 110, 28);
    }
  }

  function loop() {
    update();
    draw();
    raf = requestAnimationFrame(loop);
  }

  function onKey(e) {
    if (e.code === "Space" || e.code === "ArrowUp") {
      e.preventDefault();
      if (state === "over") {
        startGame();
      } else {
        jump();
      }
    }
  }

  startBtn.addEventListener("click", startGame);
  canvas.addEventListener("pointerdown", () => {
    if (state === "over" || state === "ready") startGame();
    else jump();
  });
  window.addEventListener("keydown", onKey);

  draw();
  raf = requestAnimationFrame(loop);
})();
