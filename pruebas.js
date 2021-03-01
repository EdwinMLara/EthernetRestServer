const axios = require('axios');
const url = 'http://10.0.0.208/update';
const url2 = 'https://jsonplaceholder.typicode.com/posts';

const bodyStart = {
    encender : 0,
    apagar : 0,
    desfase : 0
}

const bodyArray = [{...bodyStart},{...bodyStart,encender : 1}
    ,{...bodyStart,apagar : 1},{...bodyStart,desfase: 1}];

postRequestAxios = async (url,body = undefined) =>{
    let res;
    await axios({
        method:'post',
        url:url,
        headers:{'Content-type':'text/plain'},
        data: body,
        timeout: 5000
    }).then(response => {
        console.log(response.data);
        res = true;
    }).catch(error =>{
        console.log(error);
        res = false;
    })
    return res;
}

const esperar = (time) => {
    console.log("Esperando");
    return new Promise((resolver,reject) =>{
        setTimeout(()=>{
            resolver("Termino la espera");
        },time)
    })
}

(async () =>{
    for(let i=1;i<3;i++){
        console.log("calling",i);
        let response = await postRequestAxios(url,bodyArray[i])
        
        console.log(`Respuesta ${response}`);  
        
        await esperar(10000).then((msj) =>{
            console.log(msj);
        }).catch(error =>{
            console.log(error);
        });
    }
})()

