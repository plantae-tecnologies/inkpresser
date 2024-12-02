import addon from './index.cjs';

try {
    const jobDetails = addon.getJob(209);
    console.log(jobDetails);
} catch (error) {
    console.error('Error fetching job details:', error.message);
}
