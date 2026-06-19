// script.js

// ================= PILA =================
let stack = [];

function renderStack(){
  const container = document.getElementById("stackContainer");
  const message = document.getElementById("stackMessage");

  container.innerHTML = "";

  if(stack.length === 0){
    message.textContent = "La pila está vacía";
    return;
  }

  message.textContent = "";

  stack.forEach((item,index)=>{
    const div = document.createElement("div");
    div.className = "stack-item";

    if(index === stack.length - 1){
      div.innerHTML = `${item} ← Tope`;
    }else{
      div.textContent = item;
    }

    container.appendChild(div);
  });
}

function pushStack(){
  const input = document.getElementById("stackInput");
  const value = input.value.trim();

  if(value === "") return;

  stack.push(value);
  input.value = "";
  renderStack();
}

function popStack(){
  if(stack.length === 0){
    renderStack();
    return;
  }

  stack.pop();
  renderStack();
}

// ================= COLA =================
let queue = [];

function renderQueue(){
  const container = document.getElementById("queueContainer");
  const message = document.getElementById("queueMessage");

  container.innerHTML = "";

  if(queue.length === 0){
    message.textContent = "La cola está vacía";
    return;
  }

  message.textContent = "";

  queue.forEach((item,index)=>{
    const div = document.createElement("div");
    div.className = "queue-item";

    if(index === 0){
      div.innerHTML = `${item}<br><small>Frente</small>`;
    }
    else if(index === queue.length -1){
      div.innerHTML = `${item}<br><small>Final</small>`;
    }
    else{
      div.textContent = item;
    }

    container.appendChild(div);
  });
}

function enqueue(){
  const input = document.getElementById("queueInput");
  const value = input.value.trim();

  if(value === "") return;

  queue.push(value);
  input.value = "";
  renderQueue();
}

function dequeue(){
  if(queue.length === 0){
    renderQueue();
    return;
  }

  queue.shift();
  renderQueue();
}

// ================= LISTA ENLAZADA =================
let linkedList = [];

function renderList(){
  const container = document.getElementById("linkedListContainer");
  const message = document.getElementById("listMessage");

  container.innerHTML = "";

  if(linkedList.length === 0){
    message.textContent = "La lista enlazada está vacía";
    return;
  }

  message.textContent = "";

  linkedList.forEach((item,index)=>{

    const node = document.createElement("div");
    node.className = "node";

    const box = document.createElement("div");
    box.className = "node-box";

    const next = linkedList[index + 1] ? linkedList[index + 1] : "NULL";

    box.innerHTML = `
      <strong>${item}</strong>
      <hr>
      <small>next → ${next}</small>
    `;

    node.appendChild(box);

    if(index < linkedList.length -1){
      const arrow = document.createElement("div");
      arrow.className = "arrow";
      arrow.innerHTML = "→";
      node.appendChild(arrow);
    }

    container.appendChild(node);
  });
}

function insertStart(){
  const input = document.getElementById("listInput");
  const value = input.value.trim();

  if(value === "") return;

  linkedList.unshift(value);
  input.value = "";
  renderList();
}

function insertEnd(){
  const input = document.getElementById("listInput");
  const value = input.value.trim();

  if(value === "") return;

  linkedList.push(value);
  input.value = "";
  renderList();
}

function removeStart(){
  if(linkedList.length === 0){
    renderList();
    return;
  }

  linkedList.shift();
  renderList();
}

// Render inicial
renderStack();
renderQueue();
renderList();
