const Student = require('../models/studentModel')

const { getPostData } = require('../utils')
//@desc Gets All Students
//@route GET /api/students
async function getStudents(req,res){
    try{
        const students = await Student.findAll()
        res.writeHead(200,{'Content-Type':'application/json'})
        res.end(JSON.stringify(students))
    }
    catch(error){
        console.log(error)
    }
}
//@desc Gets a single students
//@route GET /api/student/:id
async function getStudent(req,res,id){
    try{
        const student = await Student.findById(id)
        if(!student){
            res.writeHead(404,{'Content-Type':'application/json'})
            res.end(JSON.stringify({message : 'Student not found'}))
        }
        else{
            res.writeHead(200,{'Content-Type':'application/json'})
            res.end(JSON.stringify(student))

        }

        
    }
    catch(error){
        console.log(error)
    }
}
//@desc Create a single student
//@route POST /api/student
async function createStudent(req,res){
    try{
        const body = await getPostData(req)

        const {name,university,faculty,phone,address,email} = JSON.parse(body)

        const student = {
            name,
            university,
            faculty,
            phone,
            address,
            email
        }

        const newStudent = await Student.create(student)
        res.writeHead(201,{'Content-Type':'application/json'})
        return res.end(JSON.stringify(newStudent))
    }
    catch(error){
        console.log(error)
    }
}

//@desc Update a single student
//@route PUT /api/student
async function updateStudent(req,res,id){
    try{
        const student = await Student.findById(id)

        if(!student){
            res.writeHead(404,{'Content-Type':'application/json'})
            res.end(JSON.stringify({message : 'Student not found'}))
        }
        else{
            const body = await getPostData(req)

            const {name,university,faculty,phone,address,email} = JSON.parse(body)

            const studentData = {
                name : name || student.name,
                university : university || student.university,
                faculty : faculty || student.faculty,
                phone : phone || student.phone,
                address : address || student.address,
                email : email || student.email
            }

            const updatedStudent = await Student.update(id,studentData)
            res.writeHead(200,{'Content-Type':'application/json'})
            return res.end(JSON.stringify(updatedStudent))
        }
    }
    catch(error){
        console.log(error)
    }
}

//@desc Remove a single student
//@route DELETE /api/student/:id
async function removeStudent(req,res,id){
    try{
        console.log(id)
        const student = await Student.findById(id)
        if(!student){
            res.writeHead(404,{'Content-Type':'application/json'})
            res.end(JSON.stringify({message : 'Student not found'}))
        }
        else{
            await Student.remove(id)
            res.writeHead(200,{'Content-Type':'application/json'})
            res.end(JSON.stringify({message:`Student ${id} removed`}))
        }
    }
    catch(error){
        console.log(error)
    }
}

//@desc Remove all students
//@route DELETE /api/student/:id
async function removeAllStudents(req,res){
    try{
        await Student.removeAll()
        res.writeHead(200,{'Content-Type':'application/json'})
        res.end(JSON.stringify({message:`All students removed`}))
        // console.log(id)
        // const student = await Student.findById(id)
        // if(!student){
        //     res.writeHead(404,{'Content-Type':'application/json'})
        //     res.end(JSON.stringify({message : 'Student not found'}))
        // }
        // else{
        //     await Student.remove(id)
        //     res.writeHead(200,{'Content-Type':'application/json'})
        //     res.end(JSON.stringify({message:`Student ${id} removed`}))
        // }
    }
    catch(error){
        console.log(error)
    }

    // try{
    //     const students = await Student.findAll()
    //     res.writeHead(200,{'Content-Type':'application/json'})
    //     res.end(JSON.stringify(students))
    // }
    // catch(error){
    //     console.log(error)
    // }
}

module.exports = {
    getStudents,
    getStudent,
    createStudent,
    updateStudent,
    removeStudent,
    removeAllStudents
}