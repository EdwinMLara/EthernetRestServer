const { retry } = require('async');
const axios = require('axios');
const { timingSafeEqual } = require('crypto');
const url = 'http://10.0.0.208/update';

const bodyStart = {
    encender : 0,
    apagar : 0,
    desfase : 0
}

const Tc = {
    tC : 0,
    teC1 : 1,
    taC1 : 4,
    teC2 : 1,
    taC2 : 3
}

const bodyArray = [{...bodyStart,encender : 1}
    ,{...bodyStart,apagar : 1},{...bodyStart,desfase: 1}];

const tCArray = [{...Tc},{...Tc,tC:1},{...Tc,tC:2}];

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

const esperar = (time,final = 0) => {
    console.log("Esperando");
    return new Promise((resolver,reject) =>{
        if(final === 2){
            reject("Se cancelo la espera");
        }else{
            setTimeout(()=>{
                resolver("Termino la espera");
            },time);
        }
    })
}

/*(async (array) =>{
    for(let i=0;i<array.length;i++){
        console.log("calling",i);
        await postRequestAxios(url,array[i]) 
        
        await esperar(10000,i).then((msj) =>{
            console.log("resolver  ",msj);
        }).catch(error =>{
            console.log("reject ", error);
        });
    }
})(bodyArray)*/

var findPeakElement = function(nums) {
    let peak = 0;
    for (let i=1;i<nums.length;i++){
        if(nums[i-1] < nums[i])
            peak = i;
    }

    return peak;
};

/*nums = [1,2,3,1];
nums2 = [1,2,1,3,5,6,4];
console.log(findPeakElement(nums))*/

var findMedianSortedArrays = function(nums1, nums2) {
    let completeArray = [...nums1,...nums2];
    completeArray.sort(function(a,b){return a-b});
    let mod = completeArray.length % 2;
    let pos = Math.floor(completeArray.length/2);
    if(mod){
        return completeArray[pos]
    }else{
        return (completeArray[pos-1] + completeArray[pos])/2;
    }
};

/*nums1 = [1,3];
nums2 = [2];

nums3 = [1,2];
nums4 = [3,4];

console.log(findMedianSortedArrays(nums1,nums2))
console.log(findMedianSortedArrays(nums3,nums4))*/

function ListNode(val,next){
    this.val = (val === undefined ? 0 : val);
    this.next = (next === undefined ? null : next);
}

ListNode.prototype.insert = function (nodeList,data){
    if(nodeList.next === null){
        nodeList.next = new ListNode(data)
    }else{
        nodeList.insert(nodeList.next,data);
    }
}

ListNode.prototype.show = function(){
    let auxNodeList = this;
    while(auxNodeList !== null){
        console.log(auxNodeList.val);
        auxNodeList = auxNodeList.next;
    }
}

list = new ListNode(1);
list.insert(list,4);
list.insert(list,5);

list2 = new ListNode(1);
list2.insert(list2,3);
list2.insert(list2,4);

list3 = new ListNode(2);
list3.insert(list3,6);

let lists = [list,list2,list3];

var mergeKLists = function(lists) {
    if(lists.length === 0 || lists.length > Math.pow(10,4)){
        return null;
    }
    let auxArrayList = []
    lists.forEach(nodeList =>{
        if(nodeList){
            while(nodeList !== null){
                auxArrayList.push(nodeList.val);
                nodeList = nodeList.next;
            } 
        }  
    });

    if(auxArrayList.length > 0){
        auxArrayList.sort(function(a,b){return a-b});

        let sortedList = new ListNode(auxArrayList[0]);
        let auxlist = sortedList;
        for(let i = 1;i<auxArrayList.length;i++){
            auxlist.next = new ListNode(auxArrayList[i]);
            auxlist = auxlist.next;
        }
        return sortedList;
    }else{
        return null;
    }
    
};

/*let list4 = new ListNode(1);
let lists2 = [null,list4]
console.log(mergeKLists([null]));*/

var strFail = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~ ";
strFail2 = strFail.slice(0,30);

for(let i = 0; i<525 ; i++){
    strFail += "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~ ";
}

strFail += strFail2;

var lengthOfLongestSubstring = function(s) {
    let maxlength = 5*Math.pow(10,4);
    let betterResult = 0;
    if(s.length >= 0 && s.length <= maxlength){
        
        let auxS = s;
        for(let i=0;i<s.length;i++){
            const auxSet = new Set();
           
            [...auxS].forEach(c =>{
                auxSet.add(c);
            });

            let srtResult = "";
            auxSet.forEach(item =>{
                return srtResult += item;
            });

            let auxSrtResult = srtResult;
            //check the right side
            while(s.indexOf(srtResult) === -1){
                srtResult = srtResult.slice(1,srtResult.length);
            }
            if(srtResult.length > betterResult){
                betterResult = srtResult.length;
            }

            //i need to check the left side
            while(s.indexOf(auxSrtResult) === -1){
                auxSrtResult = auxSrtResult.slice(0,(auxSrtResult.length-1));
            }
            if(auxSrtResult.length > betterResult){
                betterResult = auxSrtResult.length;
            }

            auxS = auxS.slice(1,auxS.length);
        }
        
    }

    return betterResult;
};

console.time("measureSubString")
console.log(lengthOfLongestSubstring(strFail));
console.timeEnd("measureSubString");

console.log("--------------------------------------");

var lengthOfLongestSubstring2 = function(s) {
    const auxSet2 = new Set(); 
    let betterResult = 0;
    let i = 0;
    for(let j=1;j<s.length;j++){
        if(auxSet2.has(s[j])){
            let maxStrLength = (j- i);
            i = j;
            auxSet2.clear();
            auxSet2.add([s[j]]);
            if(betterResult < maxStrLength){
                betterResult = maxStrLength -1;
            }
        }else{
            auxSet2.add(s[j]);
        }
    }
    return betterResult;
}

console.time("measureSubString2")
console.log(lengthOfLongestSubstring2(strFail));
console.timeEnd("measureSubString2");