const timer = x => new Promise((resolve, reject) => setTimeout(
        () => resolve(x),5000));

/*timer("mensaje").then(x => {
    console.log(x);
});

console.log("mensaje 2");

const comprarSoftware = precio => new Promise((resolve,reject) => {
    if(precio < 1000)
        resolve("Se puede comprar");
    else
        reject("el precio es muy caro");
});

comprarSoftware(900).then( msj => {
    console.log(msj);
}).catch(msj => {
    console.log(msj);
});*/

const probandoClousure = (msj) =>{
    console.log("innerFuction");
    return function (){
        console.log(msj);
    } 
} 

export {timer};
