'use strict';

let wasm

function cstrlen(mem, ptr) {
    let len = 0;
    while (mem[ptr] != 0) {
        len++;
        ptr++;
    }
    return len;
}

function cstr_by_ptr(mem_buffer, ptr) {
    const mem = new Uint8Array(mem_buffer);
    const len = cstrlen(mem, ptr);
    const bytes = new Uint8Array(mem_buffer, ptr, len);
    return new TextDecoder().decode(bytes);
}

function color_hex(color) {
    const r = ((color>>(0*8))&0xFF).toString(16).padStart(2, '0');
    const g = ((color>>(1*8))&0xFF).toString(16).padStart(2, '0');
    const b = ((color>>(2*8))&0xFF).toString(16).padStart(2, '0');
    const a = ((color>>(3*8))&0xFF).toString(16).padStart(2, '0');
    return "#"+r+g+b+a;
}

function platform_panic(file_path_ptr, line, message_ptr) {
    const buffer = wasm.instance.exports.memory.buffer;
    const file_path = cstr_by_ptr(buffer, file_path_ptr);
    const message = cstr_by_ptr(buffer, message_ptr);
    console.error(file_path+":"+line+": "+message);
}

function platform_log(message_ptr) {
    const buffer = wasm.instance.exports.memory.buffer;
    const message = cstr_by_ptr(buffer, message_ptr);
    console.log(message);
}

WebAssembly.instantiateStreaming(fetch('app.wasm'), {
    env: {
        platform_panic,
        platform_log,
    }
}).then((w) => {
    wasm = w;

    wasm.instance.exports.app_init(100, 200);
    wasm.instance.exports.foo(123123)
    const cell = wasm.instance.exports.cell;
    const getValue = wasm.instance.exports.getValue;

    console.log('From JS')
    const node = cell(442);
    console.log(node)
    console.log(getValue(node))
    console.log(getValue(0))
    console.log(getValue(1))


    const buffer = wasm.instance.exports.memory.buffer;
    console.log(wasm)

});