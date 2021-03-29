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
        // const student = students.find((s) => s.id === id)
        // resolve(student)
    })
}
function update(id, student){
    return new Promise((resolve,reject)=>{
        const index = students.findIndex((s)=>s.id === id)
        students[index] = {id, ...student}
        // const newStudent = {id:uuidv4(), ...student}
        // students.push(newStudent)
        writeDataToFile('./data/students.json',students)
        resolve(students[index])
        // const student = students.find((s) => s.id === id)
        // resolve(student)
    })
}
function remove(id){
    return new Promise((resolve,reject)=>{
        students = students.filter((s) => s.id !== id)
        //const index = students.findIndex((s)=>s.id === id)
        //students[index] = {id, ...student}
        // const newStudent = {id:uuidv4(), ...student}
        // students.push(newStudent)
        writeDataToFile('./data/students.json',students)
        resolve()
        // const student = students.find((s) => s.id === id)
        // resolve(student)
    })
}

module.exports = {
    findAll,
    findById,
    create,
    update,
    remove
}