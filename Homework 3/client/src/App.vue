<template>
    <div role="main">
        <div class="container">
            <div class="row col-md-12 d-flex justify-content-center">
                <create-student></create-student>
                <div class="row col-md-12 d-flex justify-content-center">
                    <list-students></list-students>
                </div>
            </div>
        </div>
    </div>
</template>

<script>
import CreateStudent from './components/CreateStudent';
import ListStudents from './components/ListStudents';

export default {
    name: 'app',
    components: { CreateStudent,  ListStudents},
};
</script>

<style>
</style>