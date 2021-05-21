const url = 'http://192.168.69.93/';
const axios = require('axios');

const bodyStart = {
    control:1100,
    cT:3216,
    des:23
}

const bodyArray = [{...bodyStart},{...bodyStart,control:2100}
    ,{...bodyStart,control:9910},{...bodyStart,control:9901}];

requestAxios = async (url,body) =>{
    let confReq = (body === undefined) ? 
                        { 
                            method:'GET',
                            url:url,
                            timeout: 5000
                        } : 
                        {
                            method:'POST',
                            url:url,
                            data: body,
                            timeout: 5000
                        }
    let res;
    await axios(confReq).then(response => {
        console.log(response.data);
        res = true;
    }).catch(error =>{
        console.log(error);
        res = false;
    });
    return res;
}

const esperar = (time,stop) => {
    console.log("Esperando");
    return new Promise((resolver,reject) =>{
        if(stop){
            reject("Se cancelo la espera");
        }else{
            setTimeout(()=>{
                resolver("Termino la espera");
            },time);
        }
    })
}


(async (bodyArrayP) =>{
    let len = bodyArrayP.length;
    console.log(len);
    for(let i=0;i<len;i++){
        await requestAxios(url,bodyArrayP[i]);
        let stop = (i === len - 1) ? true : false; 
        await esperar(5000,stop).then((msj) =>{
            console.log(msj);
        }).catch(error =>{
            console.log(error);
        });
        /*await requestAxios(url);
        await esperar(5000,stop).then((msj) =>{
            console.log(msj);
        }).catch(error =>{
            console.log(error);
        });*/
    }
})(bodyArray);
