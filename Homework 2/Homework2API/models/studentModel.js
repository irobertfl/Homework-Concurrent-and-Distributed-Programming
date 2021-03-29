let students = require('../data/students')
const { v4 :uuidv4 } = require('uuid')
const { writeDataToFile } = require('../utils')

function findAll(){
    return new Promise((resolve,reject)=>{
        resolve(students)
    })
}
function findById(id){
    return new Promise((resolve,reject)=>{
        const student = students.find((s) => s.id === id)
        resolve(student)
    })
}
function create(student){
    return new Promise((resolve,reject)=>{
        const newStudent = {id:uuidv4(), ...student}
        students.push(newStudent)
        writeDataToFile('./data/students.json',students)
        resolve(newStudent)
    })
}
function update(id, student){
    return new Promise((resolve,reject)=>{
        const index = students.findIndex((s)=>s.id === id)
        students[index] = {id, ...student}
        writeDataToFile('./data/students.json',students)
        resolve(students[index])
    })
}
function removeAll(){
    return new Promise((resolve,reject)=>{
        students = []
        writeDataToFile('./data/students.json',students)
        resolve()
    })
}
function remove(id){
    return new Promise((resolve,reject)=>{
        students = students.filter((s) => s.id !== id)
        writeDataToFile('./data/students.json',students)
        resolve()
    })
}

module.exports = {
    findAll,
    findById,
    create,
    update,
    remove,
    removeAll
}