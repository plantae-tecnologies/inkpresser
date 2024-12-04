import addon from './index.cjs';

try {
    console.log(addon.getJobs());
} catch (error) {
    console.error('Error fetching job details:', error.message);
}
